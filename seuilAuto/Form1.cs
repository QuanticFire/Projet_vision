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
    public partial class Form1 : Form
    {
        // Image puzzle et piece en variable globale
        Bitmap bmp_ref;
        Bitmap bmp_piece;
        ClImage Img;

        public Form1()
        {
            InitializeComponent();
        }

        private void openFileDialog1_FileOk(object sender, CancelEventArgs e)
        {

        }

        // Fonction d'affichage de l'image d'attente. Exécution dans thread
        private void affiche_wait_image()
        {
            Image wait = Image.FromFile(@"..\\..\\wait.jpg");
            Bitmap bmp_wait = new Bitmap(wait);
            imageSeuillee.Image = bmp_wait;
        }

        // Fonction de détection de piece puzzle. Est éxécutée dans un thread
        private void traitement_bouton_go()
        {
            if (dudTraitSel.Text == "Histogramme")
            {
                //imageSeuillee.Show();
                //valeurSeuilAuto.Show();


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
                    labelScore.Show();
                    labelScore2.Show();
                });
                
                // Affichagr de l'image puzzle avec détection de pièce sur l'interface
                imageSeuillee.Image = bmp_ref_copy;

                // Affichage du score
                

            }
            else if (dudTraitSel.Text == "Pattern matching")
            {
                // On efface les scores potentiellement sur l'interface
                this.Invoke((MethodInvoker)delegate ()
                {
                    labelScore.Hide();
                    labelScore2.Hide();
                });

                //imageSeuillee.Show();
                Bitmap bmp_ref_copy = new Bitmap(bmp_ref); // Création d'une copie de l'image puzzle de référence
                Img = new ClImage(); // Initialisation d'une instance de classe ClImage pour appeller le wrapper

                unsafe
                {
                    // Génération d'objets permettant de passer les data des images au wrapper
                    BitmapData bmpData = bmp_ref_copy.LockBits(new Rectangle(0, 0, bmp_ref_copy.Width, bmp_ref_copy.Height), ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb);
                    BitmapData bmpData_piece = bmp_piece.LockBits(new Rectangle(0, 0, bmp_piece.Width, bmp_piece.Height), ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb);

                    // Appel du wrapper pour traitement dans la dll
                    // Passage des données pour l'image puzzle de référence ET pour l'image piece
                    Img.PatternMatchingPtr(bmpData.Scan0, bmpData.Stride, bmp_ref_copy.Height, bmp_ref_copy.Width, bmpData_piece.Scan0, bmpData_piece.Stride, bmpData_piece.Height, bmpData_piece.Width);

                    // Traitement terminé, libération des images
                    bmp_ref_copy.UnlockBits(bmpData);
                    bmp_piece.UnlockBits(bmpData_piece);
                }
                // Lecture des coordonnées auxquelles l'algo de pattern matching détecte l'image
                double coord_x = Img.objetLibValeurChamp(1);
                double coord_y = Img.objetLibValeurChamp(0);
                //MessageBox.Show(coord_x.ToString() + " " + coord_y.ToString());

                

                // Coordonnées temporaires de détection pour implémentation de l'affichage sur l'interface de la coordonnée détectée
                //double coord_x = 1000;
                //double coord_y = 1000;

                //MessageBox.Show(coord_x.ToString());
                //MessageBox.Show(coord_y.ToString());

                // Affichagr de l'image puzzle avec détection de pièce sur l'interface
                Color myRgbColor = new Color();
                myRgbColor = Color.Red;

                for (int i = -50; i < 50; i++)
                {
                    for (int j = -50; j < 50; j++)
                    {

                        for (int shift = -10; shift < 10; shift++)
                        {
                            if ((coord_x + i > 0) && (coord_x + i < bmp_ref_copy.Width) && (coord_y + shift > 0) && (coord_y + shift < bmp_ref_copy.Height))
                            {
                                bmp_ref_copy.SetPixel(Convert.ToInt32(coord_x + i), Convert.ToInt32(coord_y + shift), myRgbColor);
                            }
                            
                        }
                            
                        for (int shift = -10; shift < 10; shift++)
                        {
                            if ((coord_x + shift > 0) && (coord_x + shift < bmp_ref_copy.Width) && (coord_y + j > 0) && (coord_y + j  < bmp_ref_copy.Height))
                            {
                                bmp_ref_copy.SetPixel(Convert.ToInt32(coord_x + shift), Convert.ToInt32(coord_y + j), myRgbColor);
                            }
                        }


                        //if (i == j)
                        //{
                        //    for (int shift = 0; shift < 10; shift++)
                        //    {
                        //        if ((coord_x + i + shift < bmp_ref_copy.Width) && (coord_x + i + shift > 0) && (coord_y + j < bmp_ref_copy.Height) && (coord_y + j > 0))
                        //        {
                        //            bmp_ref_copy.SetPixel(Convert.ToInt32(coord_x + i + shift), Convert.ToInt32(coord_y + j), myRgbColor);
                        //        }
                        //        if ((coord_x - i - shift > 0) && (coord_x - i - shift < bmp_ref_copy.Width) && (coord_y - j > 0) && (coord_y - j < bmp_ref_copy.Height))
                        //        {
                        //            bmp_ref_copy.SetPixel(Convert.ToInt32(coord_x - i - shift), Convert.ToInt32(coord_y - j), myRgbColor);
                        //        }

                        //    }

                        //    for (int shift = 0; shift < 10; shift++)
                        //    {
                        //        if ((coord_x - i + shift > 0) && (coord_x - i + shift < bmp_ref_copy.Width) && (coord_y + j < bmp_ref_copy.Height) && (coord_y + j > 0))
                        //        {
                        //            bmp_ref_copy.SetPixel(Convert.ToInt32(coord_x - i + shift), Convert.ToInt32(coord_y + j), myRgbColor);
                        //        }
                        //        if ((coord_x + i - shift > 0) && (coord_x + i - shift < bmp_ref_copy.Width) && (coord_y - j > 0) && (coord_y - j < bmp_ref_copy.Height))
                        //        {
                        //            bmp_ref_copy.SetPixel(Convert.ToInt32(coord_x + i - shift), Convert.ToInt32(coord_y - j), myRgbColor);
                        //        }

                        //    }


                        //    //bmp_ref_copy.SetPixel(Convert.ToInt32(coord_x + i), Convert.ToInt32(coord_y - j), myRgbColor);
                        //    //bmp_ref_copy.SetPixel(Convert.ToInt32(coord_x - i), Convert.ToInt32(coord_y + j), myRgbColor);
                        //}

                    }
                }

                imageSeuillee.Image = bmp_ref_copy;
            }
            else if (dudTraitSel.Text == "Histo rotation")
            {
                // On efface les scores potentiellement sur l'interface
                this.Invoke((MethodInvoker)delegate ()
                {
                    labelScore.Hide();
                    labelScore2.Hide();
                });

                Bitmap bmp_ref_rot0 = new Bitmap(bmp_ref);
                Bitmap bmp_ref_rot90 = new Bitmap(bmp_ref);
                Bitmap bmp_ref_rot180 = new Bitmap(bmp_ref);
                Bitmap bmp_ref_rot270 = new Bitmap(bmp_ref);

                Bitmap bmp_piece_rot0 = new Bitmap(bmp_piece);
                Bitmap bmp_piece_rot90 = new Bitmap(bmp_piece);
                Bitmap bmp_piece_rot180 = new Bitmap(bmp_piece);
                Bitmap bmp_piece_rot270 = new Bitmap(bmp_piece);
                bmp_piece_rot90.RotateFlip(RotateFlipType.Rotate90FlipNone);
                bmp_piece_rot180.RotateFlip(RotateFlipType.Rotate180FlipNone);
                bmp_piece_rot270.RotateFlip(RotateFlipType.Rotate270FlipNone);

                ClImage ImgRot0 = new ClImage(); // Initialisation d'une instance de classe ClImage pour appeller le wrapper
                ClImage ImgRot90 = new ClImage();
                ClImage ImgRot180 = new ClImage();
                ClImage ImgRot270 = new ClImage();

                double[] parametres = { 0 };                      

                unsafe
                {
                    // Génération d'objets permettant de passer les data des images au wrapper
                    BitmapData bmpDataRot0 = bmp_ref_rot0.LockBits(new Rectangle(0, 0, bmp_ref_rot0.Width, bmp_ref_rot0.Height), ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb);
                    BitmapData bmpData_piece_rot0 = bmp_piece_rot0.LockBits(new Rectangle(0, 0, bmp_piece_rot0.Width, bmp_piece_rot0.Height), ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb);

                    BitmapData bmpDataRot90 = bmp_ref_rot90.LockBits(new Rectangle(0, 0, bmp_ref_rot90.Width, bmp_ref_rot90.Height), ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb);
                    BitmapData bmpData_piece_rot90 = bmp_piece_rot90.LockBits(new Rectangle(0, 0, bmp_piece_rot90.Width, bmp_piece_rot90.Height), ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb);

                    BitmapData bmpDataRot180 = bmp_ref_rot180.LockBits(new Rectangle(0, 0, bmp_ref_rot180.Width, bmp_ref_rot180.Height), ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb);
                    BitmapData bmpData_piece_rot180 = bmp_piece_rot180.LockBits(new Rectangle(0, 0, bmp_piece_rot180.Width, bmp_piece_rot180.Height), ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb);

                    BitmapData bmpDataRot270 = bmp_ref_rot270.LockBits(new Rectangle(0, 0, bmp_ref_rot270.Width, bmp_ref_rot270.Height), ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb);
                    BitmapData bmpData_piece_rot270 = bmp_piece_rot270.LockBits(new Rectangle(0, 0, bmp_piece_rot270.Width, bmp_piece_rot270.Height), ImageLockMode.ReadWrite, PixelFormat.Format24bppRgb);


                    // Appel du wrapper pour traitement dans la dll
                    // Passage des données pour l'image puzzle de référence ET pour l'image piece
                    ImgRot0.traitementTestPtr(2, bmpDataRot0.Scan0, bmpDataRot0.Stride, bmpDataRot0.Height, bmpDataRot0.Width, parametres, 1, bmpData_piece_rot0.Scan0, bmpData_piece_rot0.Stride, bmpData_piece_rot0.Height, bmpData_piece_rot0.Width);
                    ImgRot90.traitementTestPtr(2, bmpDataRot90.Scan0, bmpDataRot90.Stride, bmpDataRot90.Height, bmpDataRot90.Width, parametres, 1, bmpData_piece_rot90.Scan0, bmpData_piece_rot90.Stride, bmpData_piece_rot90.Height, bmpData_piece_rot90.Width);
                    ImgRot180.traitementTestPtr(2, bmpDataRot180.Scan0, bmpDataRot180.Stride, bmpDataRot180.Height, bmpDataRot180.Width, parametres, 1, bmpData_piece_rot180.Scan0, bmpData_piece_rot180.Stride, bmpData_piece_rot180.Height, bmpData_piece_rot180.Width);
                    ImgRot270.traitementTestPtr(2, bmpDataRot270.Scan0, bmpDataRot270.Stride, bmpDataRot270.Height, bmpDataRot270.Width, parametres, 1, bmpData_piece_rot270.Scan0, bmpData_piece_rot270.Stride, bmpData_piece_rot270.Height, bmpData_piece_rot270.Width);


                    // Traitement terminé, libération des images
                    bmp_ref_rot0.UnlockBits(bmpDataRot0);
                    bmp_piece_rot0.UnlockBits(bmpData_piece_rot0);

                    bmp_ref_rot90.UnlockBits(bmpDataRot90);
                    bmp_piece_rot90.UnlockBits(bmpData_piece_rot90);

                    bmp_ref_rot180.UnlockBits(bmpDataRot180);
                    bmp_piece_rot180.UnlockBits(bmpData_piece_rot180);

                    bmp_ref_rot270.UnlockBits(bmpDataRot270);
                    bmp_piece_rot270.UnlockBits(bmpData_piece_rot270);
                }

                // Stockage des scores pour chaque orientation et les images verdict associées
                double[] scores = new double[4] { ImgRot0.objetLibValeurChamp(0), ImgRot90.objetLibValeurChamp(0), ImgRot180.objetLibValeurChamp(0), ImgRot270.objetLibValeurChamp(0) };
                Bitmap[] images_ref = new Bitmap[4] { bmp_ref_rot0, bmp_ref_rot90, bmp_ref_rot180, bmp_ref_rot270 };
                Bitmap[] images_pieces = new Bitmap[4] { bmp_piece_rot0, bmp_piece_rot90, bmp_piece_rot180, bmp_piece_rot270 };
                ClImage[] images_cl = new ClImage[4] { ImgRot0, ImgRot90, ImgRot180, ImgRot270 };
                Bitmap verdict_final;
                Bitmap bonne_orientation;
                ClImage imgcl_finale;

                // Recherche du max
                double max = -1;
                int indice_max = -1;
                for (int i = 0; i < 4; i++)
                {
                    if (scores[i] > max)
                    {
                        max = scores[i];
                        indice_max = i;
                    }
                }
                
                verdict_final = new Bitmap(images_ref[indice_max]);
                bonne_orientation = new Bitmap(images_pieces[indice_max]);
                imgcl_finale = images_cl[indice_max];

                this.Invoke((MethodInvoker)delegate ()
                {
                    labelScore.Text = imgcl_finale.objetLibValeurChamp(0).ToString() + " %";
                    labelScore2.Text = imgcl_finale.objetLibValeurChamp(1).ToString() + " %";
                    labelScore.Show();
                    labelScore2.Show();
                    
                });

                imageSeuillee.Image = verdict_final;
                pbOrientation.Image = bonne_orientation;
            }
        }

        // Quand on appuie sur le bouton "open file"
        private void buttonOuvrir_Click(object sender, EventArgs e)
        {
            //MessageBox.Show(System.IO.Directory.GetCurrentDirectory());
            //labelScore.Hide();

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
                    pbOrientation.Image = null;
                    labelScore.Hide();
                    labelScore2.Hide();
                    //valeurSeuilAuto.Hide();
                }
                catch
                {
                    MessageBox.Show("erreur !");
                }
            }
        }

        // Quand on appuie sur le bouton "go"
        private void seuillageAuto_Click(object sender, EventArgs e)
        {
            //imageSeuillee.Show();

            // Affichage de l'image d'attente. Utilisation d'un thread sinon ne fonctionne pas
            // et uniquement si mode de traitement sélectionné
            if ((dudTraitSel.Text == "Histogramme") || (dudTraitSel.Text == "Pattern matching") || (dudTraitSel.Text == "Histo rotation"))
            {
                Thread th;
                th = new Thread(new ThreadStart(affiche_wait_image));
                th.Start();
            }
            
            // Lancement du traitement de la piece. Lancement dans un autre thread, sinon cela plante l'interface et n'affiche pas l'image d'attente
            Thread th_trait;
            th_trait = new Thread(new ThreadStart(traitement_bouton_go));
            th_trait.Start();
            
            
        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {

        }

        // Quand on appuie sur le bouton pour ouvrir le puzzle...
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

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void labelScore_Click(object sender, EventArgs e)
        {

        }

        private void butForm2_Click(object sender, EventArgs e)
        {
            var form2 = new Form2(this);
            form2.Show();
            this.Hide();
        }

        private void groupBox3_Enter(object sender, EventArgs e)
        {

        }

        private void pictureBox3_Click(object sender, EventArgs e)
        {

        }
    }
}
