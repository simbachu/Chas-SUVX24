import unittest
from unittest.mock import patch, MagicMock


class TestMeanHp(unittest.TestCase):
    def setUp(self) -> None:
        # Lazy import to ensure module path resolution during test execution
        from documentation import calculate_mean_hp  # type: ignore

        self.calculate_mean_hp = calculate_mean_hp

    @patch("documentation.requests.Session.get")
    def test_generation_1_mean_hp_mocked(self, mock_get: MagicMock) -> None:
        # Arrange
        # Mock response objects with .json() and .raise_for_status()
        def make_resp(payload: dict) -> MagicMock:
            resp = MagicMock()
            resp.json.return_value = payload
            resp.raise_for_status.return_value = None
            return resp

        # First call: generation 1 species list (only 3 for test brevity)
        gen_payload = {
            "pokemon_species": [
                {"name": "bulbasaur"},
                {"name": "charmander"},
                {"name": "squirtle"},
            ]
        }

        # Next three calls: pokemon details with stats including hp
        def stats_payload(hp: int) -> dict:
            return {
                "stats": [
                    {
                        "base_stat": hp,
                        "stat": {"name": "hp"},
                    },
                    {
                        "base_stat": 0,
                        "stat": {"name": "attack"},
                    },
                ]
            }

        # Each species now triggers species endpoint first (varieties) then pokemon endpoint
        def varieties_payload(name: str) -> dict:
            return {
                "varieties": [
                    {
                        "is_default": True,
                        "pokemon": {"name": name}
                    }
                ]
            }

        mock_get.side_effect = [
            make_resp(gen_payload),
            make_resp(varieties_payload("bulbasaur")),
            make_resp(stats_payload(45)),   # bulbasaur
            make_resp(varieties_payload("charmander")),
            make_resp(stats_payload(39)),   # charmander
            make_resp(varieties_payload("squirtle")),
            make_resp(stats_payload(44)),   # squirtle
        ]

        # Act
        result = self.calculate_mean_hp([1])

        # Assert
        expected = (45 + 39 + 44) / 3.0
        self.assertAlmostEqual(result, expected)

    @patch("documentation.requests.Session.get")
    def test_multi_form_species_uses_default_variety(self, mock_get: MagicMock) -> None:
        # Arrange: generation has single species 'deoxys' with multiple forms; default is 'deoxys-normal'
        def make_resp(payload: dict) -> MagicMock:
            resp = MagicMock()
            resp.json.return_value = payload
            resp.raise_for_status.return_value = None
            return resp

        gen_payload = {
            "pokemon_species": [{"name": "deoxys"}]
        }

        varieties_payload = {
            "varieties": [
                {"is_default": False, "pokemon": {"name": "deoxys-attack"}},
                {"is_default": True,  "pokemon": {"name": "deoxys-normal"}},
                {"is_default": False, "pokemon": {"name": "deoxys-defense"}},
            ]
        }

        def stats_payload(hp: int) -> dict:
            return {
                "stats": [
                    {"base_stat": hp, "stat": {"name": "hp"}},
                ]
            }

        mock_get.side_effect = [
            make_resp(gen_payload),
            make_resp(varieties_payload),
            make_resp(stats_payload(50)),  # deoxys-normal
        ]

        # Act
        from documentation import calculate_mean_hp
        result = calculate_mean_hp([3])

        # Assert
        self.assertAlmostEqual(result, 50.0)


if __name__ == "__main__":
    unittest.main(verbosity=2)


