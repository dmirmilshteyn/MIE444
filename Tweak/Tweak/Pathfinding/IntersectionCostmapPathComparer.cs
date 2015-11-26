using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Tweak.Pathfinding
{
    class IntersectionCostmapPathComparer : IEqualityComparer<IntersectionCostmapPath>
    {
        public bool Equals(IntersectionCostmapPath x, IntersectionCostmapPath y) {
            if (x.IntersectionIds.Count != y.IntersectionIds.Count) {
                return false;
            }

            for (int i = 0; i < x.IntersectionIds.Count; i++) {
                if (x.IntersectionIds[i] != y.IntersectionIds[i]) {
                    return false;
                }
            }

            return true;
        }

        public int GetHashCode(IntersectionCostmapPath obj) {
            return -1;
        }
    }
}
