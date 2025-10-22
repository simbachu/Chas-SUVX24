import requests
import sys

def calculate_mean_hp(generations: list[int]) -> float:
    """
    Calculates the mean HP of the Pokemon across a set of Pokemon main game generations

    Note:
      Given a sufficently large list, this function will take quite a bit of time to complete.

    Examples:
      --> calculate_mean_hp([1])
        64.21
      --> calculate_mean_hp([1, 2, 3])
        66.47

    Args:
      list[int] generations: list of Pokemon main game generations
        Valid values are 1-9

    Returns:
      float: the mean HP of the Pokemon across the given generations

    Raises:
      ValueError: if the list of generations is empty or contains non-integer values or integers that do not match a Pokemon generation
      ConnectionError: if the API is not available or the request fails
    """
    if len(generations) == 0:
        raise ValueError("The list of generations is empty")
    if not all(isinstance(generation, int) for generation in generations):
        raise ValueError("The list of generations contains non-integer values")
    if not all(generation in [1, 2, 3, 4, 5, 6, 7, 8, 9] for generation in generations):
        raise ValueError("The list of generations contains integers that do not match a Pokemon generation")
    try:
        hp_values, _skipped, _total_species = _collect_hp_values_and_skips(generations)
        if len(hp_values) == 0:
            raise ValueError("No Pokemon HP data could be retrieved for the given generations")
        return sum(hp_values) / float(len(hp_values))
    except requests.exceptions.RequestException as e:
        raise ConnectionError(f"Failed to connect to the API: {e}") from e


def _collect_hp_values_and_skips(generations: list[int]) -> tuple[list[int], list[str], int]:
    """
    Internal helper that fetches HP values, tracks skipped species names, and counts total species.
    Returns (hp_values, skipped_species_names, total_species_in_generations).
    """
    session = requests.Session()
    base_url = "https://pokeapi.co/api/v2"

    hp_values: list[int] = []
    skipped: list[str] = []
    total_species: int = 0

    for generation in generations:
        # Get list of pokemon species for the generation
        gen_resp = session.get(f"{base_url}/generation/{generation}/")
        gen_resp.raise_for_status()
        gen_data = gen_resp.json()

        species_list = gen_data.get("pokemon_species", [])
        if not isinstance(species_list, list):
            continue
        total_species += len(species_list)

        for species in species_list:
            name = species.get("name") if isinstance(species, dict) else None
            if not name:
                continue

            # Fetch species details to identify default variety (handles multi-form species like deoxys)
            try:
                species_resp = session.get(f"{base_url}/pokemon-species/{name}/")
                species_resp.raise_for_status()
                species_data = species_resp.json()
            except requests.exceptions.HTTPError:
                skipped.append(name)
                continue

            varieties = species_data.get("varieties", [])
            default_pokemon_name = None
            if isinstance(varieties, list):
                for variety in varieties:
                    if not isinstance(variety, dict):
                        continue
                    if variety.get("is_default") and isinstance(variety.get("pokemon"), dict):
                        default_pokemon_name = variety["pokemon"].get("name")
                        break
            if not default_pokemon_name:
                # fallback: use species name if default variety not found
                default_pokemon_name = name

            # Fetch pokemon details to read stats from the chosen variety
            try:
                poke_resp = session.get(f"{base_url}/pokemon/{default_pokemon_name}/")
                poke_resp.raise_for_status()
                poke_data = poke_resp.json()
            except requests.exceptions.HTTPError:
                skipped.append(name)
                continue

            stats = poke_data.get("stats", [])
            if not isinstance(stats, list):
                skipped.append(name)
                continue
            # Find the HP stat entry
            hp_entry = next((s for s in stats if isinstance(s, dict) and s.get("stat", {}).get("name") == "hp"), None)
            if hp_entry is None:
                skipped.append(name)
                continue
            base_stat = hp_entry.get("base_stat")
            if isinstance(base_stat, int):
                hp_values.append(base_stat)
            else:
                skipped.append(name)

    return hp_values, skipped, total_species


if __name__ == "__main__":
    """
    Usage:
      $ python documentation.py 1
     Optionally supports comma-separated generations:
      $ python documentation.py 1,2,3

    Args:
      str arg: the argument to the script

    Returns:
      None

    Raises:
      ValueError: if the argument is not an integer or comma-separated integers
      ConnectionError: if the API is not available
    """
    if len(sys.argv) < 2:
        print("Usage: python documentation.py <generation|gen1,gen2,...>")
        sys.exit(1)

    arg = sys.argv[1]
    try:
        if "," in arg:
            gens = [int(x.strip()) for x in arg.split(",") if x.strip()]
        else:
            gens = [int(arg)]
    except ValueError:
        print("Generation must be an integer or comma-separated integers", file=sys.stderr)
        sys.exit(2)

    try:
        hp_values, skipped, total_species = _collect_hp_values_and_skips(gens)
        if not hp_values:
            raise ValueError("No Pokemon HP data could be retrieved for the given generations")
        value = sum(hp_values) / float(len(hp_values))
        print(f"Mean HP of {len(hp_values)} pokemon in generations {gens}: {value:.2f}")
        print(f"Total species in generations: {total_species}")
        if skipped:
            print("Skipped: " + ", ".join(skipped))
    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        sys.exit(3)

