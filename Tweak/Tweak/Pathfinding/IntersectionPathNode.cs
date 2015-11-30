using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Tweak.Pathfinding
{
    class IntersectionPathNode
    {
        public int IntersectionId { get; set; }
        public IntersectionType ExpectedIntersectionType { get; set; }
        public int MarkerId { get; set; }

        public IntersectionPathNode(int intersectionId, IntersectionType expectedIntersectionType, int markerId) {
            this.IntersectionId = intersectionId;
            this.ExpectedIntersectionType = expectedIntersectionType;
            this.MarkerId = markerId;
        }
    }
}
