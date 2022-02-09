namespace seuilAuto
{
    partial class Form1
    {
        /// <summary>
        /// Variable nécessaire au concepteur.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Nettoyage des ressources utilisées.
        /// </summary>
        /// <param name="disposing">true si les ressources managées doivent être supprimées ; sinon, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Code généré par le Concepteur Windows Form

        /// <summary>
        /// Méthode requise pour la prise en charge du concepteur - ne modifiez pas
        /// le contenu de cette méthode avec l'éditeur de code.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.ouvrirImage = new System.Windows.Forms.OpenFileDialog();
            this.imageDepart = new System.Windows.Forms.PictureBox();
            this.buttonOuvrir = new System.Windows.Forms.Button();
            this.panel1 = new System.Windows.Forms.Panel();
            this.panel2 = new System.Windows.Forms.Panel();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.imageSeuillee = new System.Windows.Forms.PictureBox();
            this.bSeuillageAuto = new System.Windows.Forms.Button();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            this.button_puzzle = new System.Windows.Forms.Button();
            this.dudTraitSel = new System.Windows.Forms.DomainUpDown();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.labelScore = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.labelScore2 = new System.Windows.Forms.Label();
            this.butForm2 = new System.Windows.Forms.Button();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.panelOrientation = new System.Windows.Forms.Panel();
            this.pbOrientation = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.imageDepart)).BeginInit();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.imageSeuillee)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.panelOrientation.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pbOrientation)).BeginInit();
            this.SuspendLayout();
            // 
            // ouvrirImage
            // 
            this.ouvrirImage.Filter = "Tous Fichiers | *.*";
            this.ouvrirImage.InitialDirectory = "C:\\Hubert_KONIK\\Recherche\\Database\\Images";
            this.ouvrirImage.FileOk += new System.ComponentModel.CancelEventHandler(this.openFileDialog1_FileOk);
            // 
            // imageDepart
            // 
            this.imageDepart.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.imageDepart.Cursor = System.Windows.Forms.Cursors.Default;
            this.imageDepart.Dock = System.Windows.Forms.DockStyle.Fill;
            this.imageDepart.ImageLocation = "";
            this.imageDepart.Location = new System.Drawing.Point(0, 0);
            this.imageDepart.Name = "imageDepart";
            this.imageDepart.Size = new System.Drawing.Size(268, 348);
            this.imageDepart.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.imageDepart.TabIndex = 0;
            this.imageDepart.TabStop = false;
            // 
            // buttonOuvrir
            // 
            this.buttonOuvrir.Location = new System.Drawing.Point(74, 402);
            this.buttonOuvrir.Name = "buttonOuvrir";
            this.buttonOuvrir.Size = new System.Drawing.Size(144, 35);
            this.buttonOuvrir.TabIndex = 1;
            this.buttonOuvrir.Text = "Ouvrir image piece";
            this.buttonOuvrir.UseVisualStyleBackColor = true;
            this.buttonOuvrir.Click += new System.EventHandler(this.buttonOuvrir_Click);
            // 
            // panel1
            // 
            this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel1.Controls.Add(this.imageDepart);
            this.panel1.Location = new System.Drawing.Point(23, 47);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(270, 350);
            this.panel1.TabIndex = 2;
            this.panel1.Paint += new System.Windows.Forms.PaintEventHandler(this.panel1_Paint);
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.pictureBox1);
            this.panel2.Controls.Add(this.imageSeuillee);
            this.panel2.Location = new System.Drawing.Point(54, 428);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(556, 400);
            this.panel2.TabIndex = 3;
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.pictureBox1.Cursor = System.Windows.Forms.Cursors.Default;
            this.pictureBox1.ImageLocation = "";
            this.pictureBox1.Location = new System.Drawing.Point(-535, 21);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(270, 350);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            // 
            // imageSeuillee
            // 
            this.imageSeuillee.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.imageSeuillee.Dock = System.Windows.Forms.DockStyle.Fill;
            this.imageSeuillee.Location = new System.Drawing.Point(0, 0);
            this.imageSeuillee.Name = "imageSeuillee";
            this.imageSeuillee.Size = new System.Drawing.Size(556, 400);
            this.imageSeuillee.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.imageSeuillee.TabIndex = 1;
            this.imageSeuillee.TabStop = false;
            // 
            // bSeuillageAuto
            // 
            this.bSeuillageAuto.Location = new System.Drawing.Point(577, 176);
            this.bSeuillageAuto.Name = "bSeuillageAuto";
            this.bSeuillageAuto.Size = new System.Drawing.Size(97, 75);
            this.bSeuillageAuto.TabIndex = 4;
            this.bSeuillageAuto.Text = "Go";
            this.bSeuillageAuto.UseVisualStyleBackColor = true;
            this.bSeuillageAuto.Click += new System.EventHandler(this.seuillageAuto_Click);
            // 
            // pictureBox2
            // 
            this.pictureBox2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.pictureBox2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pictureBox2.Location = new System.Drawing.Point(12, 428);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(556, 400);
            this.pictureBox2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox2.TabIndex = 7;
            this.pictureBox2.TabStop = false;
            // 
            // button_puzzle
            // 
            this.button_puzzle.Location = new System.Drawing.Point(189, 863);
            this.button_puzzle.Name = "button_puzzle";
            this.button_puzzle.Size = new System.Drawing.Size(181, 40);
            this.button_puzzle.TabIndex = 9;
            this.button_puzzle.Text = "Ouvrir puzzle";
            this.button_puzzle.UseVisualStyleBackColor = true;
            this.button_puzzle.Click += new System.EventHandler(this.button_puzzle_Click);
            // 
            // dudTraitSel
            // 
            this.dudTraitSel.Items.Add("Histogramme");
            this.dudTraitSel.Items.Add("Pattern matching");
            this.dudTraitSel.Items.Add("Histo rotation");
            this.dudTraitSel.Location = new System.Drawing.Point(6, 32);
            this.dudTraitSel.Name = "dudTraitSel";
            this.dudTraitSel.Size = new System.Drawing.Size(128, 20);
            this.dudTraitSel.TabIndex = 10;
            this.dudTraitSel.Text = "(Selection Traitement)";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.dudTraitSel);
            this.groupBox1.Location = new System.Drawing.Point(337, 176);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(173, 75);
            this.groupBox1.TabIndex = 12;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Type de traitement";
            // 
            // labelScore
            // 
            this.labelScore.AutoSize = true;
            this.labelScore.Location = new System.Drawing.Point(380, 836);
            this.labelScore.Name = "labelScore";
            this.labelScore.Size = new System.Drawing.Size(0, 13);
            this.labelScore.TabIndex = 13;
            this.labelScore.Click += new System.EventHandler(this.labelScore_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.ForeColor = System.Drawing.Color.Green;
            this.label1.Location = new System.Drawing.Point(249, 834);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(127, 13);
            this.label1.TabIndex = 14;
            this.label1.Text = "Score de similarité verict :";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(128)))), ((int)(((byte)(0)))));
            this.label2.Location = new System.Drawing.Point(250, 864);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(158, 13);
            this.label2.TabIndex = 15;
            this.label2.Text = "Score de similarité 2ème image :";
            // 
            // labelScore2
            // 
            this.labelScore2.AutoSize = true;
            this.labelScore2.Location = new System.Drawing.Point(412, 864);
            this.labelScore2.Name = "labelScore2";
            this.labelScore2.Size = new System.Drawing.Size(0, 13);
            this.labelScore2.TabIndex = 16;
            // 
            // butForm2
            // 
            this.butForm2.Location = new System.Drawing.Point(0, 0);
            this.butForm2.Name = "butForm2";
            this.butForm2.Size = new System.Drawing.Size(186, 23);
            this.butForm2.TabIndex = 17;
            this.butForm2.Text = "Traitement image non rognées";
            this.butForm2.UseVisualStyleBackColor = true;
            this.butForm2.Click += new System.EventHandler(this.butForm2_Click);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.pictureBox2);
            this.groupBox2.Location = new System.Drawing.Point(12, 29);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(610, 894);
            this.groupBox2.TabIndex = 18;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Entrées";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.label4);
            this.groupBox3.Controls.Add(this.label3);
            this.groupBox3.Controls.Add(this.panelOrientation);
            this.groupBox3.Controls.Add(this.panel2);
            this.groupBox3.Controls.Add(this.labelScore2);
            this.groupBox3.Controls.Add(this.label2);
            this.groupBox3.Controls.Add(this.labelScore);
            this.groupBox3.Controls.Add(this.label1);
            this.groupBox3.Location = new System.Drawing.Point(636, 29);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(662, 894);
            this.groupBox3.TabIndex = 19;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Sortie";
            this.groupBox3.Enter += new System.EventHandler(this.groupBox3_Enter);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(304, 412);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(72, 13);
            this.label4.TabIndex = 20;
            this.label4.Text = "Image Verdict";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(233, 15);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(212, 13);
            this.label3.TabIndex = 19;
            this.label3.Text = "Bonne Orientation (si algorithme de rotation)";
            // 
            // panelOrientation
            // 
            this.panelOrientation.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panelOrientation.Controls.Add(this.pbOrientation);
            this.panelOrientation.Location = new System.Drawing.Point(205, 31);
            this.panelOrientation.Name = "panelOrientation";
            this.panelOrientation.Size = new System.Drawing.Size(270, 350);
            this.panelOrientation.TabIndex = 17;
            // 
            // pbOrientation
            // 
            this.pbOrientation.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.pbOrientation.Cursor = System.Windows.Forms.Cursors.Default;
            this.pbOrientation.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pbOrientation.ImageLocation = "";
            this.pbOrientation.Location = new System.Drawing.Point(0, 0);
            this.pbOrientation.Name = "pbOrientation";
            this.pbOrientation.Size = new System.Drawing.Size(268, 348);
            this.pbOrientation.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pbOrientation.TabIndex = 0;
            this.pbOrientation.TabStop = false;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoScroll = true;
            this.ClientSize = new System.Drawing.Size(1315, 959);
            this.Controls.Add(this.butForm2);
            this.Controls.Add(this.bSeuillageAuto);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.button_puzzle);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.buttonOuvrir);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox3);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Form1";
            this.Text = "Projet Vision Oui-Oui";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.imageDepart)).EndInit();
            this.panel1.ResumeLayout(false);
            this.panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.imageSeuillee)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.panelOrientation.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pbOrientation)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.OpenFileDialog ouvrirImage;
        private System.Windows.Forms.PictureBox imageDepart;
        private System.Windows.Forms.Button buttonOuvrir;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.PictureBox imageSeuillee;
        private System.Windows.Forms.Button bSeuillageAuto;
        private System.Windows.Forms.PictureBox pictureBox2;
        private System.Windows.Forms.Button button_puzzle;
        private System.Windows.Forms.DomainUpDown dudTraitSel;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label labelScore;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label labelScore2;
        private System.Windows.Forms.Button butForm2;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.Panel panelOrientation;
        private System.Windows.Forms.PictureBox pbOrientation;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
    }
}

