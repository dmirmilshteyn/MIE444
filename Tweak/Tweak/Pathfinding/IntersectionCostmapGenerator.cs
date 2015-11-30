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
                    if (nodeMap[x, y].Closed) {
                        nodeMap[x, y].Distance = 0;
                    } else {
                        nodeMap[x, y].Distance = -1;
                    }
                    nodeMap[x, y].IntersectionCost = 1;
                    nodeMap[x, y].IntersectionId = -1;
                    nodeMap[x, y].MarkerId = -1;
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

        public IntersectionGraphNode[,] BuildCostmap() {
            IReadOnlyList<IntersectionMarker> selectedIntersections;
            selectedIntersections = intersectionMarkers;

            IntersectionGraphNode[,] costmap = new IntersectionGraphNode[selectedIntersections.Count, selectedIntersections.Count];

            for (int x = 0; x < selectedIntersections.Count; x++) {
                for (int y = 0; y < selectedIntersections.Count; y++) {
                    var startingMarker = selectedIntersections[x];
                    var endingMarker = selectedIntersections[y];

                    if (startingMarker.IntersectionId == endingMarker.IntersectionId) {
                        costmap[x, y] = null;
                        continue;
                    }

                    Position openStartPosition = new Position(0, 0);

                    int startX = Math.Min(startingMarker.X1, startingMarker.X2);
                    int startY = Math.Min(startingMarker.Y1, startingMarker.Y2);
                    int endX = Math.Max(startingMarker.X1, startingMarker.X2);
                    int endY = Math.Max(startingMarker.Y1, startingMarker.Y2);
                    for (int testX = startX; testX <= endX; testX++) {
                        for (int testY = startY; testY <= endY; testY++) {
                            if (testX >= 0 && testX < nodeMap.GetLength(0) &&
                                testY >= 0 && testY < nodeMap.GetLength(1) &&
                                !nodeMap[testX, testY].Closed) {
                                openStartPosition.X = testX;
                                openStartPosition.Y = testY;
                            }
                        }
                    }

                    try {
                        var path = BuildCostmapPath(openStartPosition, -1, x, y, 2).Where(p => p.IntersectionNodes.Count > 0 && p.IntersectionNodes[0].IntersectionId == startingMarker.IntersectionId && p.IntersectionNodes[p.IntersectionNodes.Count - 1].MarkerId == y).FirstOrDefault();
                        if (path != null) {
                            costmap[x, y] = new IntersectionGraphNode(path.Cost, path.IntersectionNodes[path.IntersectionNodes.Count - 1].ExpectedIntersectionType);
                        } else {
                            costmap[x, y] = null;
                        }
                    } catch (Exception ex) {
                        System.Diagnostics.Debug.WriteLine("Error");
                    }
                }
            }

            return costmap;
        }

        public IEnumerable<IntersectionCostmapPath> BuildCostmapPath(Position startPosition) {
            return BuildCostmapPath(startPosition, -1, -1, -1, -1);
        }

        public IEnumerable<IntersectionCostmapPath> BuildCostmapPath(Position startPosition, int endingIntersection, int startingIntersectionMarker, int endingIntersectionMarker, int jumpMaximum) {
            var elements = new List<IntersectionCostmapPath>();

            ResetNodeMap();

            HashSet<int> intersectionSet = new HashSet<int>();

            Queue<Position> q = new Queue<Position>();

            var startNode = GetNode(startPosition);
            startNode.Distance = 1;

            if (TestIfHittingIntersection(startPosition)) {
                startNode.IntersectionCost++;

                startNode.IntersectionId = intersectionMarkers[startingIntersectionMarker].IntersectionId;
                startNode.MarkerId = startingIntersectionMarker;
            }

            q.Enqueue(startPosition);

            while (q.Count > 0) {
                var nextPosition = q.Dequeue();
                var nextNode = GetNode(nextPosition);

                bool branchValid = true;
                foreach (var neighbourPosition in EnumerateNeighbourPositions(nextPosition)) {
                    if (neighbourPosition.X < 0 || neighbourPosition.Y < 0 || neighbourPosition.X >= nodeMap.GetLength(0) || neighbourPosition.Y >= nodeMap.GetLength(1)) {
                        continue;
                    }

                    var neighbourNode = GetNode(neighbourPosition);

                    if (neighbourNode.Distance == -1) {
                        neighbourNode.Distance = nextNode.Distance + 1;
                        neighbourNode.Parent = nextNode;
                        neighbourNode.IntersectionCost = nextNode.IntersectionCost;

                        TestIfHittingIntersection(neighbourPosition);
                        if (neighbourNode.IntersectionId == intersectionMarkers[startingIntersectionMarker].IntersectionId) {
                            neighbourNode.MarkerId = startingIntersectionMarker;
                        }

                        if (neighbourNode.MarkerId != -1) {
                            if (!intersectionSet.Contains(neighbourNode.IntersectionId)) {
                                intersectionSet.Add(neighbourNode.IntersectionId);
                            }

                            if (jumpMaximum > -1 && DetermineIntersectionCost(neighbourPosition) == jumpMaximum) {
                                branchValid = false;
                                elements.Add(ReconstructPath(neighbourPosition, true));
                            }
                            //if (jumpMaximum > -1 && neighbourNode.IntersectionCost >= jumpMaximum) {
                            //    branchValid = false;
                            //}

                            //if (jumpMaximum > -1 && neighbourNode.IntersectionCost == jumpMaximum) {
                            //    elements.Add(ReconstructPath(neighbourPosition, true));
                            //}

                            if ((endingIntersection != -1 && neighbourNode.IntersectionId == endingIntersection)
                                || (endingIntersectionMarker != -1 && neighbourNode.MarkerId == endingIntersectionMarker)) {
                                elements.Add(ReconstructPath(neighbourPosition, true));
                                branchValid = false;
                            }
                        }

                        if (branchValid) {
                            q.Enqueue(neighbourPosition);
                        }
                    }
                }
            }

            return elements;
            //yield return new IntersectionCostmapPath(0, new List<int>(), false);
        }

        private bool TestIfHittingIntersection(Position neighbourPosition) {
            int hittingIntersectionMarker = FindHittingIntersectionMarker(intersectionMarkers, neighbourPosition);

            if (hittingIntersectionMarker != -1) {
                var neighbourNode = GetNode(neighbourPosition);

                int hittingIntersection = intersectionMarkers[hittingIntersectionMarker].IntersectionId;

                neighbourNode.IntersectionId = hittingIntersection;
                neighbourNode.MarkerId = hittingIntersectionMarker;
                int lastIntersectionId = FindLastIntersectionId(neighbourPosition);
                if (lastIntersectionId != -1 && lastIntersectionId != neighbourNode.IntersectionId &&
                    IsIntersectionInChain(neighbourPosition, hittingIntersection) == false) {
                    // Only increase the cost if the last intersection marker was different
                    neighbourNode.IntersectionCost++;
                }
            }

            return (hittingIntersectionMarker != -1);
        }

        private int DetermineIntersectionCost(Position testPosition) {
            int cost = 0;

            List<int> intersectionIds = new List<int>();

            var currentNode = GetNode(testPosition);
            while (currentNode != null) {
                if (currentNode.IntersectionId > -1 && intersectionIds.Where(n => n == currentNode.IntersectionId).Count() == 0) {
                    cost++;

                    intersectionIds.Add(currentNode.IntersectionId);
                }

                currentNode = currentNode.Parent;
            }

            return cost;
        }

        private bool IsIntersectionInChain(Position testPosition, int intersectionId) {
            var currentNode = GetNode(testPosition);
            while (currentNode != null) {
                if (currentNode.IntersectionId == intersectionId) {
                    return true;
                }

                currentNode = currentNode.Parent;
            }

            return false;
        }

        private int FindLastIntersectionId(Position testPosition) {
            var currentNode = GetNode(testPosition).Parent;
            while (currentNode != null) {
                if (currentNode.IntersectionId != -1) {
                    return currentNode.IntersectionId;
                }
                currentNode = currentNode.Parent;
            }

            return -1;
        }

        private IntersectionCostmapPath ReconstructPath(Position endingPosition, bool completePath) {
            int cost = 0;

            List<IntersectionPathNode> intersectionIds = new List<IntersectionPathNode>();

            IntersectionCostmapNode currentNode = GetNode(endingPosition);
            while (currentNode != null) {
                int intersectionId = currentNode.IntersectionId;
                if (intersectionId > -1 && intersectionIds.Where(n => n.IntersectionId == intersectionId).Count() == 0) {
                    intersectionIds.Add(new IntersectionPathNode(intersectionId, intersectionMarkers[currentNode.MarkerId].IntersectionType, currentNode.MarkerId));
                }

                cost++;
                currentNode = currentNode.Parent;
            }

            intersectionIds.Reverse();
            return new IntersectionCostmapPath(cost, intersectionIds, completePath);
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

        private int FindHittingIntersectionMarker(IReadOnlyList<IntersectionMarker> intersections, Position testPosition) {

            for (int i = 0; i < intersections.Count; i++) {
                var marker = intersections[i];
                if (IsOnPoint(testPosition.X, testPosition.Y, marker.X1, marker.Y1, marker.X2, marker.Y2)) {
                    return i;
                }
            }

            return -1;
        }

        bool IsOnPoint(int x, int y, int x1, int y1, int x2, int y2) {
            if (x <= Math.Max(x1, x2) && x >= Math.Min(x1, x2)
                && y <= Math.Max(y1, y2) && y >= Math.Min(y1, y2)) {
                return true;
            }

            return false;
        }
    }
}
