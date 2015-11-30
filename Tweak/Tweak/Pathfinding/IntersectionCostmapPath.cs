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
        public List<IntersectionPathNode> IntersectionNodes { get; set; }

        public bool Complete { get; set; }

        public IntersectionCostmapPath(int cost, List<IntersectionPathNode> intersectionNodes, bool complete) {
            this.Cost = cost;
            this.IntersectionNodes = intersectionNodes;
            this.Complete = complete;
        }
    }
}
