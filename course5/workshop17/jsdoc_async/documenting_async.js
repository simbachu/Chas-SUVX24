/** PokeAPI https://pokeapi.co/
 *  Call https://pokeapi.co/api/v2/pokemon?limit=100000 to get the list of all pokemon.
 *  Then pick any 3. Return the name and number in a json object array.
*/

/**
 * Extract numeric Pokémon ID from a PokeAPI resource URL.
 * @param {string} url - PokeAPI resource URL, e.g. "https://pokeapi.co/api/v2/pokemon/1/"
 * @returns {number} The numeric ID.
 */
function extractIdFromUrl(url) {
  const parts = url.split('/').filter(Boolean);
  const idStr = parts[parts.length - 1];
  const id = Number(idStr);
  return Number.isNaN(id) ? -1 : id;
}

/**
 * Fetch Pokémon (optionally by type) and return exactly three as name/number pairs.
 * If no type is provided, picks from all Pokémon.
 *
 * @async
 * @param {Object} [options]
 * @param {string} [options.type] - Optional PokeAPI type name (e.g. "grass", "fire").
 * @param {boolean} [options.genwunner=false] - When true, restrict to Gen 1 (first 151). Applies to full list and type-filtered results.
 * @returns {Promise<Array<{name: string, number: number}>>}
 *
 * @example
 * // Get any 3 grass-type Pokémon
 * const threeGrass = await getThreePokemon({ type: 'grass' });
 * // Get any 3 Pokémon regardless of type (e.g., mew, ditto, wartortle)
 * const anyThree = await getThreePokemon();
 */
async function getThreePokemon(options = {}) {
  const { type, genwunner = false } = options;

  let list = [];
  if (type) {
    // Fetch by type: https://pokeapi.co/api/v2/type/{type}
    const res = await fetch(`https://pokeapi.co/api/v2/type/${encodeURIComponent(type)}`);
    if (!res.ok) {
      throw new Error(`Failed to fetch type '${type}': ${res.status} ${res.statusText}`);
    }
    const data = await res.json();
    // data.pokemon: [{ slot, pokemon: { name, url } }]
    list = (data.pokemon || []).map((p) => p.pokemon);
    if (genwunner) {
      list = list.filter((p) => extractIdFromUrl(p.url) > 0 && extractIdFromUrl(p.url) <= 151);
    }
  } else {
    // Fallback to full list (151 if genwunner mode)
    const limit = genwunner ? 151 : 100000;
    const res = await fetch(`https://pokeapi.co/api/v2/pokemon?limit=${limit}`);
    if (!res.ok) {
      throw new Error(`Failed to fetch pokemon list: ${res.status} ${res.statusText}`);
    }
    const data = await res.json();
    list = data.results || [];
  }

  // Pick three random unique entries
  const indices = new Set();
  while (indices.size < Math.min(3, list.length)) {
    indices.add(Math.floor(Math.random() * list.length));
  }
  const picked = Array.from(indices).map((i) => ({
    name: list[i].name,
    number: extractIdFromUrl(list[i].url)
  }));

  return picked;
}

// Export for external usage
module.exports = { getThreePokemon };

// If run directly, demo the function, getting 3 grass pokemon from gen 1.
if (require.main === module) {
  (async () => {
    try {
      const result = await getThreePokemon({genwunner: true, type: 'grass'});
      console.log(result);
    } catch (err) {
      console.error(err);
      process.exitCode = 1;
    }
  })();
}

