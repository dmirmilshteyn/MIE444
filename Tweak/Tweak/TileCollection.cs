using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;

namespace Tweak
{
    public class TileCollection
    {
        Tile[] tiles;

        public Tile[] Tiles {
            get { return tiles; }
            set { tiles = value; }
        }

        public int Width { get; set; }

        public int Height { get; set; }

        public Tile this[int x, int y] {
            get { return tiles[y * Width + x]; }
        }

        public void Initialize(Constants constants) {
            if (tiles == null) {
                this.Width = (int)(constants.MapWidth / constants.MapResolution);
                this.Height = (int)(constants.MapHeight / constants.MapResolution);

                tiles = new Tile[this.Width * this.Height];
                for (int i = 0; i < tiles.Length; i++) {
                    tiles[i] = new Tile();
                }
            }
        }

    }
}
