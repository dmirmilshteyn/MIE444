using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using System.Xml.Serialization;

namespace Tweak
{
    public class Tile : ObservableObject
    {
        [XmlIgnore]
        public ICommand ToggleStateCommand { get; private set; }

        bool filled;
        public bool Filled {
            get { return filled; }
            set {
                filled = value;
                RaisePropertyChanged();
            }
        }

        public Tile() {
            ToggleStateCommand = new Command(ToggleStateCallback);
        }

        private void ToggleStateCallback() {
            this.Filled = !this.Filled;
        }
    }
}
