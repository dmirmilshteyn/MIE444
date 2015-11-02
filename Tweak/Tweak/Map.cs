using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Imaging;
using System.Xml.Serialization;

namespace Tweak
{
    public class Map : ObservableObject
    {
        WriteableBitmap gridBitmap;
        [XmlIgnore]
        public WriteableBitmap GridBitmap {
            get { return gridBitmap; }
            set {
                gridBitmap = value;
                RaisePropertyChanged();
            }
        }

        TileCollection tiles;
        public TileCollection Tiles {
            get { return tiles; }
            set {
                tiles = value;
                RaisePropertyChanged();
            }
        }

        public void InitializeMap(Constants constants) {
            if (Tiles == null) {
                Tiles = new TileCollection();
                Tiles.Initialize(constants);
            }
        }

        public byte[] Export() {
            int exportedWidth = Tiles.Width / 8;
            int exportedHeight = Tiles.Height / 8;

            byte[] values = new byte[exportedWidth * exportedHeight];
            short index = -1;
            for (int x = 0; x < Tiles.Width; x++) {
                for (int y = 0; y < Tiles.Height; y++) {
                    if ((y * Tiles.Width + x) % 8 == 0) {
                        index++;
                    }

                    short position = (short)(index / 8);
                    short offset = (short)(index % 8);

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

        public void RefreshGridBitmap() {
            using (gridBitmap.GetBitmapContext()) {

            }
        }
    }
}
