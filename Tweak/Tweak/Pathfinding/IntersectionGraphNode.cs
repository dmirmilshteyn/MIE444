using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Tweak.Pathfinding
{
    class IntersectionGraphNode
    {
        public int Cost { get; set; }
        public IntersectionType IntersectionType { get; set; }

        public IntersectionGraphNode(int cost, IntersectionType intersectionType) {
            this.Cost = cost;
            this.IntersectionType = intersectionType;
        }
    }
}
