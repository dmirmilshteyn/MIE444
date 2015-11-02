using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Tweak
{
    class Command : ICommand
    {
        public event EventHandler CanExecuteChanged;

        Action execute;

        public Command(Action execute) {
            this.execute = execute;
        }

        public bool CanExecute(object parameter) {
            return true;
        }

        public void Execute(object parameter) {
            execute();
        }
    }
}
