using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The User Control item template is documented at http://go.microsoft.com/fwlink/?LinkId=234236

namespace Tweak.Bluetooth
{
    public sealed partial class RemoteMapViewer : UserControl
    {
        public RemoteMap RemoteMap {
            get { return (RemoteMap)GetValue(RemoteMapProperty); }
            set { SetValue(RemoteMapProperty, value); }
        }

        public static readonly DependencyProperty RemoteMapProperty = DependencyProperty.Register("RemoteMap", typeof(RemoteMap), typeof(RemoteMapViewer), new PropertyMetadata(0));

        public RemoteMapViewer() {
            this.InitializeComponent();
        }

        private void CanvasControl_Draw(Microsoft.Graphics.Canvas.UI.Xaml.CanvasControl sender, Microsoft.Graphics.Canvas.UI.Xaml.CanvasDrawEventArgs args) {
            if (RemoteMap != null) {

                for (int x = 0;x < RemoteMap.Width; x++) {
                    for (int y = 0;y < RemoteMap.Height; y++) {
                       if (RemoteMap.MapTiles[x, y]) {
                            args.DrawingSession.DrawRectangle(new Rect(x, y, 1, 1), Colors.Black);
                        }
                    }
                }

            }
        }
    }
}
