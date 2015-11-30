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
            if (x.IntersectionNodes.Count != y.IntersectionNodes.Count) {
                return false;
            }

            for (int i = 0; i < x.IntersectionNodes.Count; i++) {
                if (x.IntersectionNodes[i] != y.IntersectionNodes[i]) {
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
