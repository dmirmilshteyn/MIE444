using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Tweak.Pathfinding
{
    class IntersectionCostmapGenerator
    {
        IntersectionCostmapNode[,] nodeMap;
        IReadOnlyList<IntersectionMarker> intersectionMarkers;

        public IntersectionCostmapGenerator(Map map) {
            BuildNodeMap(map);

            this.intersectionMarkers = map.IntersectionMarkers;
        }

        private void BuildNodeMap(Map map) {
            nodeMap = new IntersectionCostmapNode[map.Tiles.Width, map.Tiles.Height];

            for (int x = 0; x < map.Tiles.Width; x++) {
                for (int y = 0; y < map.Tiles.Height; y++) {
                    nodeMap[x, y] = new IntersectionCostmapNode(x, y);

                    if (!map.Tiles[x, y].Filled) {
                        nodeMap[x, y].Closed = true;
                        nodeMap[x, y].Distance = 0;
                    }
                }
            }
        }

        private void ResetNodeMap() {
            for (int x = 0; x < nodeMap.GetLength(0); x++) {
                for (int y = 0; y < nodeMap.GetLength(1); y++) {
                    nodeMap[x, y].Distance = -1;
                    nodeMap[x, y].Parent = null;
                }
            }
        }

        private List<IntersectionMarker> CountUniqueIntersectionMarkers() {
            HashSet<int> markerId = new HashSet<int>();

            List<IntersectionMarker> markers = new List<IntersectionMarker>();

            foreach (var marker in intersectionMarkers) {
                if (!markerId.Contains(marker.IntersectionId)) {
                    markers.Add(marker);
                    markerId.Add(marker.IntersectionId);
                }
            }

            return markers;
        }

        public int[,] BuildCostmap(bool onlyUnique) {
            IReadOnlyList<IntersectionMarker> selectedIntersections;
            if (onlyUnique) {
                selectedIntersections = CountUniqueIntersectionMarkers();
            } else {
                selectedIntersections = intersectionMarkers;
            }

            int[,] costmap = new int[selectedIntersections.Count, selectedIntersections.Count];

            for (int x = 0; x < selectedIntersections.Count; x++) {
                for (int y = 0; y < selectedIntersections.Count; y++) {
                    var startingMarker = selectedIntersections[x];
                    var endingMarker = selectedIntersections[y];

                    if (x == y) {
                        costmap[x, y] =-1;
                        continue;
                    }

                    int cost = BuildCostmapPath(new Position(startingMarker.X1, startingMarker.Y1), endingMarker.IntersectionId);
                    costmap[x, y] = cost;
                }
            }

            return costmap;
        }

        public int BuildCostmapPath(Position startPosition) {
            return BuildCostmapPath(startPosition, -1);
        }

        public int BuildCostmapPath(Position startPosition, int endingIntersection) {
            ResetNodeMap();

            HashSet<int> intersectionSet = new HashSet<int>();

            Queue<Position> q = new Queue<Position>();

            var startNode = GetNode(startPosition);
            startNode.Distance = 0;
            q.Enqueue(startPosition);

            while (q.Count > 0) {
                var nextPosition = q.Dequeue();
                var nextNode = GetNode(nextPosition);

                foreach (var neighbourPosition in EnumerateNeighbourPositions(nextPosition)) {
                    if (neighbourPosition.X < 0 || neighbourPosition.Y < 0 || neighbourPosition.X >= nodeMap.GetLength(0) || neighbourPosition.Y >= nodeMap.GetLength(1)) {
                        continue;
                    }

                    var neighbourNode = GetNode(neighbourPosition);

                    if (neighbourNode.Distance == -1) {
                        neighbourNode.Distance = nextNode.Distance + 1;
                        neighbourNode.Parent = nextNode;

                        int hittingIntersection = FindHittingIntersection(intersectionMarkers, neighbourPosition);

                        if (hittingIntersection != -1) {
                            if (!intersectionSet.Contains(hittingIntersection)) {
                                intersectionSet.Add(hittingIntersection);
                            }

                            if (endingIntersection != -1 && hittingIntersection == endingIntersection) {
                                return DetermineCostOfPath(neighbourPosition);
                            }
                        }

                        q.Enqueue(neighbourPosition);
                    }
                }
            }

            return 0;
        }

        private int DetermineCostOfPath(Position endingPosition) {
            int cost = 0;

            IntersectionCostmapNode currentNode = GetNode(endingPosition);
            while (currentNode.Parent != null) {
                cost++;
                currentNode = currentNode.Parent;
            }

            return cost;
        }

        private IntersectionCostmapNode GetNode(Position position) {
            return nodeMap[position.X, position.Y];
        }

        private IEnumerable<Position> EnumerateNeighbourPositions(Position position) {
            // Top row
            yield return new Position(position.X - 1, position.Y - 1);
            yield return new Position(position.X, position.Y - 1);
            yield return new Position(position.X + 1, position.Y - 1);
            // Middle row
            yield return new Position(position.X - 1, position.Y);
            yield return new Position(position.X + 1, position.Y);
            // Bottom row
            yield return new Position(position.X - 1, position.Y + 1);
            yield return new Position(position.X, position.Y + 1);
            yield return new Position(position.X + 1, position.Y + 1);
        }

        private int FindHittingIntersection(IReadOnlyCollection<IntersectionMarker> intersections, Position testPosition) {
            foreach (var marker in intersections) {
                if (IsOnPoint(testPosition.X, testPosition.Y, marker.X1, marker.Y1, marker.X2, marker.Y2)) {
                    return marker.IntersectionId;
                }
            }

            return -1;
        }

        bool IsOnPoint(int x, int y, int x1, int y1, int x2, int y2) {

            if (x <= Math.Max(x1, x2) && x >= Math.Min(x1, x2)
                && y <= Math.Max(y1, y2) && y >= Math.Min(y1, y2)) {
                return true;
            }

            //float a = (y2 - y1) / (x2 - x1);
            //float b = y1 - a * x1;
            //if (Math.Abs(y - (a * x + b)) < 0.001) {
            //    return true;
            //}

            return false;
        }
    }
}
