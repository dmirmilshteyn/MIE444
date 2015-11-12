using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Tweak.Pathfinding
{
    class Pathfinder
    {
        // Pathfinding based on https://en.wikipedia.org/wiki/A*_search_algorithm#Pseudocode

        Node[,] nodeMap;

        public Pathfinder(Map map) {
            BuildNodeMap(map);
        }

        private void BuildNodeMap(Map map) {
            nodeMap = new Node[map.Tiles.Width, map.Tiles.Height];

            for (int x = 0; x < map.Tiles.Width; x++) {
                for (int y = 0; y < map.Tiles.Height; y++) {
                    nodeMap[x, y] = new Node(x, y);

                    if (!map.Tiles[x, y].Filled) {
                        nodeMap[x, y].Closed = true;
                    }
                }
            }
        }

        public IReadOnlyList<Position> AStar(Position startPosition, Position goalPosition) {
            List<Position> openSet = new List<Position>();
            List<Position> closedSet = new List<Position>();

            int openMax = 1;
            int closedMax = 0;

            openSet.Add(startPosition);

            int expiryLevel = 0;

            while (openSet.Count > 0) {
                Position currentPosition = DetermineNextNode(openSet);
                if (currentPosition.X == goalPosition.X && currentPosition.Y == goalPosition.Y) {
                    return ReconstructPath(currentPosition);
                }

                Node currentNode = GetNode(currentPosition);

                if (expiryLevel == 0 && closedSet.Count >= 400 ||
                    expiryLevel == 1 && closedSet.Count >= 600 ||
                    expiryLevel == 2 && closedSet.Count >= 800 ||
                    expiryLevel == 3 && closedSet.Count >= 900) {
                    ExpireOldNodes(closedSet);
                    expiryLevel++;
                }

                // Remove the current node from the open list
                RemovePosition(openSet, currentPosition);
                closedSet.Add(currentPosition);

                if (closedSet.Count > closedMax) {
                    closedMax = closedSet.Count;
                }

                foreach (var neighbourPosition in EnumerateNeighbourPositions(currentPosition)) {
                    if (neighbourPosition.X < 0 || neighbourPosition.Y < 0 || neighbourPosition.X >= nodeMap.GetLength(0) || neighbourPosition.Y >= nodeMap.GetLength(1)) {
                        continue;
                    }

                    Node neighbour = GetNode(neighbourPosition);

                    if (HasPosition(closedSet, neighbourPosition) || neighbour.Closed) {
                        continue;
                    }

                    double tentative_g_score = currentNode.G + CalculateDistanceBetween(currentPosition, neighbourPosition); // Length of this path
                    if (!HasPosition(openSet, neighbourPosition)) { // Discover a new node
                        openSet.Add(neighbourPosition);

                        if (openSet.Count > openMax) {
                            openMax = openSet.Count;
                        }
                    } else if (tentative_g_score > neighbour.G) {
                        continue; // This is not a better path
                    }

                    // This path is the best until now. Record it!
                    neighbour.Parent = currentNode;
                    neighbour.G = tentative_g_score;
                    neighbour.H = 0; // Use Dijkstra's algorithm for now
                }
            }

            return new List<Position>();
        }

        private void ExpireOldNodes(IList<Position> set) {
            for (int i = set.Count - 1; i >= 0; i--) {
                Position currentPosition = set[i];

                bool openNeighbour = false;  
                foreach (var neighbourPosition in EnumerateNeighbourPositions(currentPosition)) {
                    if (!(neighbourPosition.X < 0 ||
                        neighbourPosition.Y < 0 ||
                        neighbourPosition.X >= nodeMap.GetLength(0) ||
                        neighbourPosition.Y >= nodeMap.GetLength(1) ||
                        HasPosition(set, neighbourPosition) ||
                        GetNode(neighbourPosition).Closed
                        )) {

                        openNeighbour = true;
                    }
                }

                if (!openNeighbour) {
                    set.RemoveAt(i);
                }
            }
        }

        private void RemovePosition(IList<Position> set, Position position) {
            for (int i = set.Count - 1; i >= 0; i--) {
                if (set[i].X == position.X && set[i].Y == position.Y) {
                    set.RemoveAt(i);
                    return;
                }
            }
        }

        private bool HasPosition(IList<Position> set, Position position) {
            foreach (var setPosition in set) {
                if (setPosition.X == position.X && setPosition.Y == position.Y) {
                    return true;
                }
            }

            return false;
        }

        private IReadOnlyList<Position> ReconstructPath(Position endingPosition) {
            List<Position> path = new List<Position>();

            Node currentNode = GetNode(endingPosition);
            while (currentNode.Parent != null) {
                path.Add(new Position(currentNode.X, currentNode.Y));

                currentNode = currentNode.Parent;
            }

            path.Add(new Position(currentNode.X, currentNode.Y));

            path.Reverse();
            return path;
        }

        private Node GetNode(Position position) {
            return nodeMap[position.X, position.Y];
        }

        private Position DetermineNextNode(List<Position> openSet) {
            Node bestNode = null;
            Position bestPosition = null;

            foreach (var position in openSet) {
                Node testNode = GetNode(position);
                if (bestNode == null || testNode.F < bestNode.F) {
                    bestNode = testNode;
                    bestPosition = position;
                }
            }

            return bestPosition;
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

        private double CalculateDistanceBetween(Position positionA, Position positionB) {
            return Math.Sqrt(Math.Pow(positionB.X - positionA.X, 2) + Math.Pow(positionB.Y - positionA.Y, 2));
        }

    }
}
