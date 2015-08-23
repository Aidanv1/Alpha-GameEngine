using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using GameTools.Tools;
namespace GameTools
{
    public partial class ModelConverterForm : Form
    {
        public ModelConverterForm()
        {
            InitializeComponent();
            messageLabel.Text = "";
        }

        private void exportButton_Click(object sender, EventArgs e)
        {
            if(!ModelConverter.ConvertFile(inputFileBox.Text, outputFileBox.Text))
            {
                messageLabel.Text = "Failed to export model file.";
            }
            else
            {
                messageLabel.Text = "Successfully exported model file.";
            }

        }
    }
}
