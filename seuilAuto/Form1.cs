﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing.Imaging;

using System.Runtime.InteropServices;
using Traitement;

namespace seuilAuto
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void openFileDialog1_FileOk(object sender, CancelEventArgs e)
        {

        }

        private void buttonOuvrir_Click(object sender, EventArgs e)
        {
            if (ouvrirImage.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    Bitmap bmp;
                    Image img = Image.FromFile(ouvrirImage.FileName);
                    bmp = new Bitmap(img);

                    imageDepart.Width = bmp.Width;
                    imageDepart.Height = bmp.Height;
                    // pour centrer image dans panel
                    if (imageDepart.Width < panel1.Width)
                        imageDepart.Left = (panel1.Width - imageDepart.Width) / 2;

                    if (imageDepart.Height < panel1.Height)
                        imageDepart.Top = (panel1.Height - imageDepart.Height) / 2;

                    imageDepart.Image = bmp;

                    imageSeuillee.Hide();
                    valeurSeuilAuto.Hide();
                }
                catch
                {
                    MessageBox.Show("erreur !");
                }
            }
        }

        private void seuillageAuto_Click(object sender, EventArgs e)
        {
            // traitement donc transférer data bmp vers C++

            imageSeuillee.Show();
            valeurSeuilAuto.Show();

            Bitmap bmp = new Bitmap(imageDepart.Image);
            ClImage Img = new ClImage();

            unsafe
            {
                BitmapData bmpData = bmp.LockBits(new Rectangle(0, 0, bmp.Width, bmp.Height), ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb);
                Img.objetLibDataImgPtr(1, bmpData.Scan0, bmpData.Stride, bmp.Height, bmp.Width);
                // 1 champ texte retour C++, le seuil auto
                bmp.UnlockBits(bmpData);
            }

            valeurSeuilAuto.Text = Img.objetLibValeurChamp(0).ToString();

            imageSeuillee.Width = bmp.Width;
            imageSeuillee.Height = bmp.Height;

            // pour centrer image dans panel
            if (imageSeuillee.Width < panel1.Width)
                imageSeuillee.Left = (panel1.Width - imageSeuillee.Width) / 2;

            if (imageSeuillee.Height < panel1.Height)
                imageSeuillee.Top = (panel1.Height - imageSeuillee.Height) / 2;

            // transférer C++ vers bmp
            imageSeuillee.Image = bmp;
        }
    }
}
