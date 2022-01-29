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
            this.imageSeuillee = new System.Windows.Forms.PictureBox();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.bSeuillageAuto = new System.Windows.Forms.Button();
            this.buttonPuzzle = new System.Windows.Forms.Button();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            this.button_puzzle = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.imageDepart)).BeginInit();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.imageSeuillee)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
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
            this.imageDepart.ImageLocation = "";
            this.imageDepart.Location = new System.Drawing.Point(0, 0);
            this.imageDepart.Name = "imageDepart";
            this.imageDepart.Size = new System.Drawing.Size(0, 0);
            this.imageDepart.TabIndex = 0;
            this.imageDepart.TabStop = false;
            // 
            // buttonOuvrir
            // 
            this.buttonOuvrir.Location = new System.Drawing.Point(252, 405);
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
            this.panel1.Location = new System.Drawing.Point(198, 38);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(270, 350);
            this.panel1.TabIndex = 2;
            this.panel1.Paint += new System.Windows.Forms.PaintEventHandler(this.panel1_Paint);
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.pictureBox1);
            this.panel2.Location = new System.Drawing.Point(733, 17);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(556, 400);
            this.panel2.TabIndex = 3;
            // 
            // imageSeuillee
            // 
            this.imageSeuillee.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.imageSeuillee.Location = new System.Drawing.Point(733, 17);
            this.imageSeuillee.Name = "imageSeuillee";
            this.imageSeuillee.Size = new System.Drawing.Size(556, 400);
            this.imageSeuillee.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.imageSeuillee.TabIndex = 1;
            this.imageSeuillee.TabStop = false;
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.pictureBox1.Cursor = System.Windows.Forms.Cursors.Default;
            this.pictureBox1.ImageLocation = "";
            this.pictureBox1.Location = new System.Drawing.Point(0, 0);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(0, 0);
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            // 
            // bSeuillageAuto
            // 
            this.bSeuillageAuto.Location = new System.Drawing.Point(582, 181);
            this.bSeuillageAuto.Name = "bSeuillageAuto";
            this.bSeuillageAuto.Size = new System.Drawing.Size(48, 34);
            this.bSeuillageAuto.TabIndex = 4;
            this.bSeuillageAuto.Text = "Go";
            this.bSeuillageAuto.UseVisualStyleBackColor = true;
            this.bSeuillageAuto.Click += new System.EventHandler(this.seuillageAuto_Click);
            // 
            // buttonPuzzle
            // 
            this.buttonPuzzle.Location = new System.Drawing.Point(0, 0);
            this.buttonPuzzle.Name = "buttonPuzzle";
            this.buttonPuzzle.Size = new System.Drawing.Size(75, 23);
            this.buttonPuzzle.TabIndex = 8;
            // 
            // pictureBox2
            // 
            this.pictureBox2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.pictureBox2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pictureBox2.Location = new System.Drawing.Point(394, 478);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(556, 400);
            this.pictureBox2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox2.TabIndex = 7;
            this.pictureBox2.TabStop = false;
            // 
            // button_puzzle
            // 
            this.button_puzzle.Location = new System.Drawing.Point(582, 884);
            this.button_puzzle.Name = "button_puzzle";
            this.button_puzzle.Size = new System.Drawing.Size(181, 40);
            this.button_puzzle.TabIndex = 9;
            this.button_puzzle.Text = "Ouvrir puzzle";
            this.button_puzzle.UseVisualStyleBackColor = true;
            this.button_puzzle.Click += new System.EventHandler(this.button_puzzle_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1496, 959);
            this.Controls.Add(this.imageSeuillee);
            this.Controls.Add(this.button_puzzle);
            this.Controls.Add(this.pictureBox2);
            this.Controls.Add(this.buttonPuzzle);
            this.Controls.Add(this.bSeuillageAuto);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.buttonOuvrir);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Form1";
            this.Text = "Seuillage en deux classes";
            ((System.ComponentModel.ISupportInitialize)(this.imageDepart)).EndInit();
            this.panel1.ResumeLayout(false);
            this.panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.imageSeuillee)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
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
        private System.Windows.Forms.Button buttonPuzzle;
        private System.Windows.Forms.PictureBox pictureBox2;
        private System.Windows.Forms.Button button_puzzle;
    }
}

