using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing.Imaging;
using System.IO;

using System.Runtime.InteropServices;
using Traitement;

namespace seuilAuto
{
    public partial class Form2 : Form
    {
        Form form1_;

        // Image puzzle et piece en variable globale
        Bitmap bmp_ref;
        Bitmap bmp_piece;
        ClImage Img;

        public Form2(Form form1)
        {
            InitializeComponent();
            form1_ = form1;
        }

        private void traitement_bouton_go()
        {

        }

        private void butForm1_Click(object sender, EventArgs e)
        {
            this.Hide();
            form1_.Show();
        }

        private void buttonOuvrir_Click(object sender, EventArgs e)
        {
            //MessageBox.Show(System.IO.Directory.GetCurrentDirectory());
            labelScore.Hide();

            if (ouvrirImage.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    // Récupération de l'image
                    Image img = Image.FromFile(ouvrirImage.FileName);
                    bmp_piece = new Bitmap(img);

                    // Ajustement de la taille du panel par rapport à l'image
                    imageDepart.Width = bmp_piece.Width;
                    imageDepart.Height = bmp_piece.Height;

                    // pour centrer image dans panel
                    if (imageDepart.Width < panel1.Width)
                        imageDepart.Left = (panel1.Width - imageDepart.Width) / 2;

                    if (imageDepart.Height < panel1.Height)
                        imageDepart.Top = (panel1.Height - imageDepart.Height) / 2;

                    // Affichage de l'image
                    imageDepart.Image = bmp_piece;

                    // Quand on affiche une nouvelle image, on cache l'ancienne image traitée pour garder une cohérence visuelle sur l'interface
                    //imageSeuillee.Hide();
                    imageSeuillee.Image = null;
                    //valeurSeuilAuto.Hide();
                }
                catch
                {
                    MessageBox.Show("erreur !");
                }
            }
        }

        private void bSeuillageAuto_Click(object sender, EventArgs e)
        {
            Bitmap bmp_ref_copy = new Bitmap(bmp_ref); // Création d'une copie de l'image puzzle de référence
            Img = new ClImage(); // Initialisation d'une instance de classe ClImage pour appeller le wrapper

            // affectation des paramètres (a supprimer en faisant attention aux changement wrapper et dll, ne sert a rien)
            double[] parametres = { 0 }; // = new double[parametersTextBox.Lines.Length];
                                         //for (int i = 0; i < parametersTextBox.Lines.Length; i++)
                                         //   parametres[i] = Convert.ToDouble(parametersTextBox.Lines[i]);

            unsafe
            {
                // Génération d'objets permettant de passer les data des images au wrapper
                BitmapData bmpData = bmp_ref_copy.LockBits(new Rectangle(0, 0, bmp_ref_copy.Width, bmp_ref_copy.Height), ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb);
                BitmapData bmpData_piece = bmp_piece.LockBits(new Rectangle(0, 0, bmp_piece.Width, bmp_piece.Height), ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb);

                // Appel du wrapper pour traitement dans la dll
                // Passage des données pour l'image puzzle de référence ET pour l'image piece
                Img.traitementTestPtr(2, bmpData.Scan0, bmpData.Stride, bmp_ref_copy.Height, bmp_ref_copy.Width, parametres, 1, bmpData_piece.Scan0, bmpData_piece.Stride, bmpData_piece.Height, bmpData_piece.Width);

                // ancien commentaire : 1 champ texte retour C++, le seuil auto
                // Traitement terminé, libération des images
                bmp_ref_copy.UnlockBits(bmpData);
                bmp_piece.UnlockBits(bmpData_piece);
            }

            this.Invoke((MethodInvoker)delegate ()
            {
                labelScore.Text = Img.objetLibValeurChamp(0).ToString() + " %";
                labelScore2.Text = Img.objetLibValeurChamp(1).ToString() + " %";
            });

            // Affichagr de l'image puzzle avec détection de pièce sur l'interface
            imageSeuillee.Image = bmp_ref_copy;
        }

        private void button_puzzle_Click(object sender, EventArgs e)
        {
            if (ouvrirImage.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    // Initialisations
                    Image img = Image.FromFile(ouvrirImage.FileName);
                    bmp_ref = new Bitmap(img);

                    pictureBox2.Image = bmp_ref; // Affichage de l'image

                    //imageSeuillee.Hide();
                    //valeurSeuilAuto.Hide();
                }
                catch
                {
                    MessageBox.Show("erreur !");
                }
            }
        }

        private void Form2_Load(object sender, EventArgs e)
        {

        }

    }
}
