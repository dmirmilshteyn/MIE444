using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Tweak
{
    public class Tile : ObservableObject
    {
        bool filled;
        public bool Filled {
            get { return filled; }
            set {
                filled = value;
                RaisePropertyChanged();
            }
        }
    }
}
