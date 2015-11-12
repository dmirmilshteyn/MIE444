using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;

namespace Tweak
{
    public class TileCollection : ObservableObject
    {
        Tile[] tiles;

        public Tile[] Tiles {
            get { return tiles; }
            set {
                tiles = value;
                RaisePropertyChanged();
            }
        }

        int width;
        public int Width {
            get { return width; }
            set {
                width = value;
                RaisePropertyChanged();
            }
        }

        int height;
        public int Height {
            get { return height; }
            set {
                height = value;
                RaisePropertyChanged();
            }
        }

        public Tile this[int x, int y] {
            get { return tiles[y * Width + x]; }
        }

        public void Initialize(Constants constants) {
            if (tiles == null) {
                this.Width = (int)Math.Ceiling(constants.MapWidth / constants.MapResolution);
                this.Height = (int)Math.Ceiling(constants.MapHeight / constants.MapResolution);

                Tile[] localTiles = new Tile[this.Width * this.Height];
                for (int i = 0; i < localTiles.Length; i++) {
                    localTiles[i] = new Tile();
                }

                this.Tiles = localTiles;
            }

            this.Tiles = this.Tiles;
        }

    }
}
