using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Tweak.Pathfinding
{
    class IntersectionCostmapPath
    {
        public int Cost { get; set; }
        public List<int> IntersectionIds { get; set; }

        public bool Complete { get; set; }

        public IntersectionCostmapPath(int cost, List<int> intersectionIds, bool complete) {
            this.Cost = cost;
            this.IntersectionIds = intersectionIds;
            this.Complete = complete;
        }
    }
}
