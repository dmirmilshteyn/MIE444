using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Tweak.Bluetooth
{
    public class RemoteMap
    {
        public event EventHandler MapRefreshed;

        public bool[,] MapTiles { get; private set; }

        public int Width {
            get { return MapTiles.GetLength(0); }
        }

        public int Height {
            get { return MapTiles.GetLength(1); }
        }

        public void InitializeMap(bool[,] mapTiles) {
            this.MapTiles = mapTiles;

            MapRefreshed?.Invoke(this, EventArgs.Empty);
        }
    }
}
