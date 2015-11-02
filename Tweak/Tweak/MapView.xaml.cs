using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Tweak
{
    /// <summary>
    /// Interaction logic for MapView.xaml
    /// </summary>
    public partial class MapView : UserControl
    {
        public MapView() {
            InitializeComponent();
        }

        private void Rectangle_MouseEnter(object sender, MouseEventArgs e) {
            if (e.LeftButton == MouseButtonState.Pressed) {
                Rectangle rect = (Rectangle)sender;
                Tile tile = (Tile)rect.DataContext;

                tile.Filled = true;
            } else if (e.RightButton == MouseButtonState.Pressed) {
                Rectangle rect = (Rectangle)sender;
                Tile tile = (Tile)rect.DataContext;

                tile.Filled = false;
            }
        }

        private void Rectangle_MouseDown(object sender, MouseButtonEventArgs e) {
            if (e.LeftButton == MouseButtonState.Pressed) {
                Rectangle rect = (Rectangle)sender;
                Tile tile = (Tile)rect.DataContext;

                tile.Filled = true;
            } else if (e.RightButton == MouseButtonState.Pressed) {
                Rectangle rect = (Rectangle)sender;
                Tile tile = (Tile)rect.DataContext;

                tile.Filled = false;
            }
        }
    }
}
