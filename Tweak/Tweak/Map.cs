using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;

namespace Tweak
{
    public class Map : ObservableObject
    {
        TileCollection tiles;
        public TileCollection Tiles {
            get { return tiles; }
            set {
                tiles = value;
                RaisePropertyChanged();
            }
        }

        ObservableCollection<Intersection> intersections;
        public ObservableCollection<Intersection> Intersections {
            get { return intersections; }
            set {
                intersections = value;
                RaisePropertyChanged();
            }
        }

        ObservableCollection<IntersectionMarker> intersectionMarkers;
        public ObservableCollection<IntersectionMarker> IntersectionMarkers {
            get { return intersectionMarkers; }
            set {
                intersectionMarkers = value;
                RaisePropertyChanged();
            }
        }

        public void InitializeMap(Constants constants) {
            if (Tiles == null) {
                Tiles = new TileCollection();
                Tiles.Initialize(constants);
            }
            if (Intersections == null) {
                Intersections = new ObservableCollection<Intersection>();
            }
            if (IntersectionMarkers == null) {
                IntersectionMarkers = new ObservableCollection<IntersectionMarker>();
            }
        }

        public byte[] Export() {
            byte[] values = new byte[(int)Math.Ceiling(Tiles.Width * Tiles.Height / 8d)];
            for (int y = 0; y < Tiles.Height; y++) {
                for (int x = 0; x < Tiles.Width; x++) {
                    int tileNum = y * Tiles.Width + x;

                    short position = (short)(tileNum / 8);
                    short offset = (short)(tileNum % 8);

                    int internal_value = values[position];

                    if (Tiles[x, y].Filled) {
                        internal_value = internal_value | 1 << offset;
                    } else {
                        internal_value = internal_value & ~(1 << offset);
                    }

                    values[position] = (byte)internal_value;
                }
            }

            return values;
        }
    }
}
