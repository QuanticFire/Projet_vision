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
            this.valeurSeuilAuto = new System.Windows.Forms.TextBox();
            this.parametersTextBox = new System.Windows.Forms.TextBox();
            ((System.ComponentModel.ISupportInitialize)(this.imageDepart)).BeginInit();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.imageSeuillee)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
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
            this.imageDepart.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.imageDepart.Name = "imageDepart";
            this.imageDepart.Size = new System.Drawing.Size(0, 0);
            this.imageDepart.TabIndex = 0;
            this.imageDepart.TabStop = false;
            // 
            // buttonOuvrir
            // 
            this.buttonOuvrir.Location = new System.Drawing.Point(216, 545);
            this.buttonOuvrir.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.buttonOuvrir.Name = "buttonOuvrir";
            this.buttonOuvrir.Size = new System.Drawing.Size(100, 28);
            this.buttonOuvrir.TabIndex = 1;
            this.buttonOuvrir.Text = "OpenFile";
            this.buttonOuvrir.UseVisualStyleBackColor = true;
            this.buttonOuvrir.Click += new System.EventHandler(this.buttonOuvrir_Click);
            // 
            // panel1
            // 
            this.panel1.AutoScroll = true;
            this.panel1.Controls.Add(this.imageDepart);
            this.panel1.Location = new System.Drawing.Point(16, 21);
            this.panel1.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(533, 492);
            this.panel1.TabIndex = 2;
            // 
            // panel2
            // 
            this.panel2.AutoScroll = true;
            this.panel2.Controls.Add(this.imageSeuillee);
            this.panel2.Controls.Add(this.pictureBox1);
            this.panel2.Location = new System.Drawing.Point(629, 21);
            this.panel2.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(533, 492);
            this.panel2.TabIndex = 3;
            // 
            // imageSeuillee
            // 
            this.imageSeuillee.Location = new System.Drawing.Point(0, 0);
            this.imageSeuillee.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.imageSeuillee.Name = "imageSeuillee";
            this.imageSeuillee.Size = new System.Drawing.Size(533, 492);
            this.imageSeuillee.TabIndex = 1;
            this.imageSeuillee.TabStop = false;
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.pictureBox1.Cursor = System.Windows.Forms.Cursors.Default;
            this.pictureBox1.ImageLocation = "";
            this.pictureBox1.Location = new System.Drawing.Point(0, 0);
            this.pictureBox1.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(0, 0);
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            // 
            // bSeuillageAuto
            // 
            this.bSeuillageAuto.Location = new System.Drawing.Point(557, 246);
            this.bSeuillageAuto.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.bSeuillageAuto.Name = "bSeuillageAuto";
            this.bSeuillageAuto.Size = new System.Drawing.Size(64, 42);
            this.bSeuillageAuto.TabIndex = 4;
            this.bSeuillageAuto.Text = "Go";
            this.bSeuillageAuto.UseVisualStyleBackColor = true;
            this.bSeuillageAuto.Click += new System.EventHandler(this.seuillageAuto_Click);
            // 
            // valeurSeuilAuto
            // 
            this.valeurSeuilAuto.Location = new System.Drawing.Point(875, 545);
            this.valeurSeuilAuto.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.valeurSeuilAuto.Name = "valeurSeuilAuto";
            this.valeurSeuilAuto.Size = new System.Drawing.Size(81, 22);
            this.valeurSeuilAuto.TabIndex = 0;
            this.valeurSeuilAuto.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            this.valeurSeuilAuto.Visible = false;
            // 
            // parametersTextBox
            // 
            this.parametersTextBox.Location = new System.Drawing.Point(16, 520);
            this.parametersTextBox.Multiline = true;
            this.parametersTextBox.Name = "parametersTextBox";
            this.parametersTextBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.parametersTextBox.Size = new System.Drawing.Size(103, 56);
            this.parametersTextBox.TabIndex = 5;
            this.parametersTextBox.Text = "21\r\n15";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1172, 588);
            this.Controls.Add(this.parametersTextBox);
            this.Controls.Add(this.valeurSeuilAuto);
            this.Controls.Add(this.bSeuillageAuto);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.buttonOuvrir);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.Name = "Form1";
            this.Text = "Seuillage en deux classes";
            ((System.ComponentModel.ISupportInitialize)(this.imageDepart)).EndInit();
            this.panel1.ResumeLayout(false);
            this.panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.imageSeuillee)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

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
        private System.Windows.Forms.TextBox valeurSeuilAuto;
        private System.Windows.Forms.TextBox parametersTextBox;
    }
}

