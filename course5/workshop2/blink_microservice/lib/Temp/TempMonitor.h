#pragma once

//! Temperature monitoring with inner fast ring (0.5s) and outer aggregated ring (10s).
//! - Samples raw sensor into inner ring; computes sigma-clipped averages into outer ring.
//! - Alerts are evaluated only on outer aggregated values.

#include <stdint.h>
#include <limits>
#include <math.h>

#include "TempInterface.h"

class TempMonitor {
public:
	//! Inner sampling interval in milliseconds.
	static constexpr uint16_t kDefaultSampleIntervalMs = 100;
	//! Inner ring capacity (60s / 0.1s = 600 samples).
	static constexpr uint16_t kMaxCapacity = 600; // inner ring capacity (60s / 0.1s)
	//! Outer ring capacity (60 minutes at 10s intervals).
	static constexpr uint16_t kOuterCapacity = 360; // 60 minutes at 10s intervals
	//! Allowed alert range (sensor expected range)
	static constexpr float kAllowedMinC = -50.0f;
	static constexpr float kAllowedMaxC = 100.0f;

	//! Construct monitor with a sensor interface and ring configs.
	TempMonitor(TempInterface &sensor,
	           uint16_t sample_interval_ms = kDefaultSampleIntervalMs,
	           uint16_t ring_capacity = kMaxCapacity)
		: sensor_(sensor),
		  sample_interval_ms_(sample_interval_ms),
		  capacity_(ring_capacity > kMaxCapacity ? kMaxCapacity : ring_capacity) {}

	//! Initialize the underlying sensor.
	void begin() { sensor_.begin(); }

	//! Sample the sensor when the interval elapses and update rings/alerts.
	void sample_if_due(uint32_t now_ms) {
		if ((uint32_t)(now_ms - last_sample_ms_) < sample_interval_ms_) return;
		last_sample_ms_ = now_ms;
		float t = sensor_.read_celsius();
		buffer_[ring_index_] = t;
		ring_index_ = (ring_index_ + 1) % capacity_;
		if (ring_size_ < capacity_) ring_size_++;

		// Outer aggregation every 10s using sigma-clipped inner average
		if ((uint32_t)(now_ms - last_outer_aggregate_ms_) >= 10000U) {
			last_outer_aggregate_ms_ = now_ms;
			float clipped = average_last_window_sigma_clipped(1.0f);
			outer_buffer_[outer_index_] = clipped;
			outer_index_ = (outer_index_ + 1) % kOuterCapacity;
			if (outer_size_ < kOuterCapacity) outer_size_++;
			// Track historic min/max on sigma-trimmed outer values only
			if (!isnan(clipped)) {
				if (clipped < min_historic_) min_historic_ = clipped;
				if (clipped > max_historic_) max_historic_ = clipped;
			}
			// Check alerts only on outer values
			if (has_alert_) {
				if (!outer_is_within_range(clipped)) {
					alert_violated_ = true;
				}
			}
		}
	}

	//! Simple mean of inner ring (no outlier rejection).
	float average_last_window() const {
		if (ring_size_ == 0) return NAN;
		float sum = 0.0f;
		for (uint16_t i = 0; i < ring_size_; ++i) sum += buffer_[i];
		return sum / (float)ring_size_;
	}

	//! Sigma-clipped average of inner ring; discards samples farther than k·stddev from mean.
	float average_last_window_sigma_clipped(float k) const {
		if (ring_size_ == 0) return NAN;
		if (ring_size_ == 1) return buffer_[0];
		// First pass: mean
		float sum = 0.0f;
		for (uint16_t i = 0; i < ring_size_; ++i) sum += buffer_[i];
		float mean = sum / (float)ring_size_;
		// Second pass: stddev
		float var_sum = 0.0f;
		for (uint16_t i = 0; i < ring_size_; ++i) {
			float d = buffer_[i] - mean;
			var_sum += d * d;
		}
		float stddev = sqrtf(var_sum / (float)ring_size_);
		if (stddev == 0.0f) return mean;
		// Third pass: clipped mean
		float clip_sum = 0.0f;
		uint16_t kept = 0;
		float threshold = k * stddev;
		for (uint16_t i = 0; i < ring_size_; ++i) {
			float d = buffer_[i] - mean;
			if (fabsf(d) <= threshold) {
				clip_sum += buffer_[i];
				kept++;
			}
		}
		return kept == 0 ? mean : (clip_sum / (float)kept);
	}

	//! Set lower alert threshold (inclusive) evaluated on outer ring values.
	void set_alert_min(float c) { alert_min_c_ = c; has_alert_ = true; }
	//! Set upper alert threshold (inclusive) evaluated on outer ring values.
	void set_alert_max(float c) { alert_max_c_ = c; has_alert_ = true; }
	//! Get current alert thresholds if configured.
	bool get_alerts(float &min_c, float &max_c) const { if (!has_alert_) return false; min_c = alert_min_c_; max_c = alert_max_c_; return true; }
	//! True if any outer aggregated value fell outside thresholds since configured.
	bool was_alert_violated() const { return alert_violated_; }
	//! Reset the alert violation flag.
	void clear_alert_violation() { alert_violated_ = false; }

public:
	//! Check if a value is within configured thresholds.
	bool outer_is_within_range(float c) const {
		bool above_min = (alert_min_c_ == -std::numeric_limits<float>::infinity()) || (c >= alert_min_c_);
		bool below_max = (alert_max_c_ == std::numeric_limits<float>::infinity()) || (c <= alert_max_c_);
		return above_min && below_max;
	}

	//! True when the inner ring has filled its configured capacity.
	bool is_window_full() const { return ring_size_ >= capacity_; }

	//! Get historical minimum from raw samples; returns false if no samples.
	bool get_min_historic(float &out_celsius) const {
		if (min_historic_ == std::numeric_limits<float>::infinity()) return false;
		out_celsius = min_historic_;
		return true;
	}

	//! Get historical maximum from raw samples; returns false if no samples.
	bool get_max_historic(float &out_celsius) const {
		if (max_historic_ == -std::numeric_limits<float>::infinity()) return false;
		out_celsius = max_historic_;
		return true;
	}

private:
	//! Underlying sensor interface.
	TempInterface &sensor_;
	//! Inner sampling interval in milliseconds.
	uint16_t sample_interval_ms_;
	//! Configured inner ring capacity.
	uint16_t capacity_;
	//! Inner ring buffer of raw samples.
	float buffer_[kMaxCapacity];
	//! Current inner ring size and head index.
	uint16_t ring_size_ = 0;
	uint16_t ring_index_ = 0;
	//! Historical min/max from raw samples.
	float min_historic_ = std::numeric_limits<float>::infinity();
	float max_historic_ = -std::numeric_limits<float>::infinity();
	//! Timestamp of last inner sample.
	uint32_t last_sample_ms_ = 0;
	//! Outer ring of aggregated, sigma-clipped values.
	float outer_buffer_[kOuterCapacity] = {0};
	uint16_t outer_size_ = 0;
	uint16_t outer_index_ = 0;
	//! Timestamp of last outer aggregation.
	uint32_t last_outer_aggregate_ms_ = 0;
	//! Alert configuration and state.
	bool has_alert_ = false;
	float alert_min_c_ = -std::numeric_limits<float>::infinity();
	float alert_max_c_ = std::numeric_limits<float>::infinity();
	bool alert_violated_ = false;
};


