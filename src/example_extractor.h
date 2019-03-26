/**\file example_extractor.h
 *
 * Conversion of ExampleRef to Example
 *
 *
 *  Created on: Mar 25, 2019
 *      Author: dkoes
 */

#ifndef EXAMPLE_EXTRACTOR_H_
#define EXAMPLE_EXTRACTOR_H_

#include <memory>
#include <vector>
#include <unordered_map>
#include "example.h"
#include "exampleref_providers.h"
#include "atom_typer.h"
#include "coordinateset.h"

namespace libmolgrid {

/** \brief Converts an ExampleRef to and Example
 * Loads (potentially cached) data and applies atom typers to create coordinate sets.
 Takes care of in-memory caching (optional) and also supports memory mapped gnina cache files
(incurring slight overhead on recalculation of atom types in
 exchange for substantially less real mem usage).

 Can take multiple atom typers, in which case they are applied in order, with the last being repeated.

 */
class ExampleExtractor {

    using CoordCache = std::unordered_map<const char*, CoordinateSet>;

    std::vector<std::shared_ptr<AtomTyper> > typers;
    std::vector<CoordCache> coord_caches; //different typers have duplicated caches

    bool use_cache = true;
    bool addh = true;

    void set_coords(const char *fname, unsigned which, CoordinateSet& coord);
  public:

    template<typename ...Typers>
    ExampleExtractor(const ExampleProviderSettings& settings, Typers... typrs): typers{typers} {
      coord_caches.resize(typers.size());
      if(typers.size() == 0) throw std::invalid_argument("Need at least one atom typer for example extractor");
    }
    virtual ~ExampleExtractor() {}

    /// Extract ref into ex
    virtual void extract(const ExampleRef& ref, Example& ex);
};

} /* namespace libmolgrid */

#endif /* EXAMPLE_EXTRACTOR_H_ */