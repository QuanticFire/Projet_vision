namespace seuilAuto
{
    partial class Form2
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.butForm1 = new System.Windows.Forms.Button();
            this.ouvrirImage = new System.Windows.Forms.OpenFileDialog();
            this.imageDepart = new System.Windows.Forms.PictureBox();
            this.panel1 = new System.Windows.Forms.Panel();
            this.buttonOuvrir = new System.Windows.Forms.Button();
            this.button_puzzle = new System.Windows.Forms.Button();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            this.bSeuillageAuto = new System.Windows.Forms.Button();
            this.labelScore2 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.labelScore = new System.Windows.Forms.Label();
            this.imageSeuillee = new System.Windows.Forms.PictureBox();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.panel2 = new System.Windows.Forms.Panel();
            this.panelRogne = new System.Windows.Forms.Panel();
            this.pbRogne = new System.Windows.Forms.PictureBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.gbTraitement = new System.Windows.Forms.GroupBox();
            this.dudTraitSel = new System.Windows.Forms.DomainUpDown();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.imageDepart)).BeginInit();
            this.panel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.imageSeuillee)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.panel2.SuspendLayout();
            this.panelRogne.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pbRogne)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.gbTraitement.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // butForm1
            // 
            this.butForm1.Location = new System.Drawing.Point(-1, 0);
            this.butForm1.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.butForm1.Name = "butForm1";
            this.butForm1.Size = new System.Drawing.Size(295, 28);
            this.butForm1.TabIndex = 0;
            this.butForm1.Text = "Retour vers traitement image non rognées";
            this.butForm1.UseVisualStyleBackColor = true;
            this.butForm1.Click += new System.EventHandler(this.butForm1_Click);
            // 
            // ouvrirImage
            // 
            this.ouvrirImage.Filter = "Tous Fichiers | *.*";
            this.ouvrirImage.InitialDirectory = "C:\\Hubert_KONIK\\Recherche\\Database\\Images";
            // 
            // imageDepart
            // 
            this.imageDepart.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.imageDepart.Cursor = System.Windows.Forms.Cursors.Default;
            this.imageDepart.Dock = System.Windows.Forms.DockStyle.Fill;
            this.imageDepart.ImageLocation = "";
            this.imageDepart.Location = new System.Drawing.Point(0, 0);
            this.imageDepart.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.imageDepart.Name = "imageDepart";
            this.imageDepart.Size = new System.Drawing.Size(357, 428);
            this.imageDepart.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.imageDepart.TabIndex = 0;
            this.imageDepart.TabStop = false;
            // 
            // panel1
            // 
            this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel1.Controls.Add(this.imageDepart);
            this.panel1.Location = new System.Drawing.Point(24, 34);
            this.panel1.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(359, 430);
            this.panel1.TabIndex = 19;
            // 
            // buttonOuvrir
            // 
            this.buttonOuvrir.Location = new System.Drawing.Point(97, 473);
            this.buttonOuvrir.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.buttonOuvrir.Name = "buttonOuvrir";
            this.buttonOuvrir.Size = new System.Drawing.Size(192, 43);
            this.buttonOuvrir.TabIndex = 17;
            this.buttonOuvrir.Text = "Ouvrir image piece";
            this.buttonOuvrir.UseVisualStyleBackColor = true;
            this.buttonOuvrir.Click += new System.EventHandler(this.buttonOuvrir_Click);
            // 
            // button_puzzle
            // 
            this.button_puzzle.Location = new System.Drawing.Point(284, 1041);
            this.button_puzzle.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.button_puzzle.Name = "button_puzzle";
            this.button_puzzle.Size = new System.Drawing.Size(192, 49);
            this.button_puzzle.TabIndex = 23;
            this.button_puzzle.Text = "Ouvrir puzzle";
            this.button_puzzle.UseVisualStyleBackColor = true;
            this.button_puzzle.Click += new System.EventHandler(this.button_puzzle_Click);
            // 
            // pictureBox2
            // 
            this.pictureBox2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.pictureBox2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pictureBox2.Location = new System.Drawing.Point(24, 542);
            this.pictureBox2.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(741, 492);
            this.pictureBox2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox2.TabIndex = 22;
            this.pictureBox2.TabStop = false;
            // 
            // bSeuillageAuto
            // 
            this.bSeuillageAuto.Location = new System.Drawing.Point(783, 182);
            this.bSeuillageAuto.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.bSeuillageAuto.Name = "bSeuillageAuto";
            this.bSeuillageAuto.Size = new System.Drawing.Size(120, 92);
            this.bSeuillageAuto.TabIndex = 21;
            this.bSeuillageAuto.Text = "Go";
            this.bSeuillageAuto.UseVisualStyleBackColor = true;
            this.bSeuillageAuto.Click += new System.EventHandler(this.bSeuillageAuto_Click);
            // 
            // labelScore2
            // 
            this.labelScore2.AutoSize = true;
            this.labelScore2.Location = new System.Drawing.Point(1888, 681);
            this.labelScore2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.labelScore2.Name = "labelScore2";
            this.labelScore2.Size = new System.Drawing.Size(0, 17);
            this.labelScore2.TabIndex = 28;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(128)))), ((int)(((byte)(0)))));
            this.label2.Location = new System.Drawing.Point(1672, 681);
            this.label2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(213, 17);
            this.label2.TabIndex = 27;
            this.label2.Text = "Score de similarité 2ème image :";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.ForeColor = System.Drawing.Color.Green;
            this.label1.Location = new System.Drawing.Point(1671, 644);
            this.label1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(170, 17);
            this.label1.TabIndex = 26;
            this.label1.Text = "Score de similarité verict :";
            // 
            // labelScore
            // 
            this.labelScore.AutoSize = true;
            this.labelScore.Location = new System.Drawing.Point(1845, 646);
            this.labelScore.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.labelScore.Name = "labelScore";
            this.labelScore.Size = new System.Drawing.Size(0, 17);
            this.labelScore.TabIndex = 25;
            // 
            // imageSeuillee
            // 
            this.imageSeuillee.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.imageSeuillee.Dock = System.Windows.Forms.DockStyle.Fill;
            this.imageSeuillee.Location = new System.Drawing.Point(0, 0);
            this.imageSeuillee.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.imageSeuillee.Name = "imageSeuillee";
            this.imageSeuillee.Size = new System.Drawing.Size(741, 492);
            this.imageSeuillee.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.imageSeuillee.TabIndex = 18;
            this.imageSeuillee.TabStop = false;
            this.imageSeuillee.Click += new System.EventHandler(this.imageSeuillee_Click);
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.pictureBox1.Cursor = System.Windows.Forms.Cursors.Default;
            this.pictureBox1.ImageLocation = "";
            this.pictureBox1.Location = new System.Drawing.Point(-713, 26);
            this.pictureBox1.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(360, 431);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.imageSeuillee);
            this.panel2.Controls.Add(this.pictureBox1);
            this.panel2.Location = new System.Drawing.Point(76, 542);
            this.panel2.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(741, 492);
            this.panel2.TabIndex = 20;
            // 
            // panelRogne
            // 
            this.panelRogne.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panelRogne.Controls.Add(this.pbRogne);
            this.panelRogne.Location = new System.Drawing.Point(268, 54);
            this.panelRogne.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.panelRogne.Name = "panelRogne";
            this.panelRogne.Size = new System.Drawing.Size(359, 430);
            this.panelRogne.TabIndex = 30;
            // 
            // pbRogne
            // 
            this.pbRogne.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.pbRogne.Cursor = System.Windows.Forms.Cursors.Default;
            this.pbRogne.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pbRogne.ImageLocation = "";
            this.pbRogne.Location = new System.Drawing.Point(0, 0);
            this.pbRogne.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.pbRogne.Name = "pbRogne";
            this.pbRogne.Size = new System.Drawing.Size(357, 428);
            this.pbRogne.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.pbRogne.TabIndex = 0;
            this.pbRogne.TabStop = false;
            this.pbRogne.Click += new System.EventHandler(this.pbRogne_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.gbTraitement);
            this.groupBox1.Controls.Add(this.button_puzzle);
            this.groupBox1.Controls.Add(this.buttonOuvrir);
            this.groupBox1.Controls.Add(this.panel1);
            this.groupBox1.Controls.Add(this.pictureBox2);
            this.groupBox1.ForeColor = System.Drawing.SystemColors.ControlText;
            this.groupBox1.Location = new System.Drawing.Point(17, 52);
            this.groupBox1.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Padding = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.groupBox1.Size = new System.Drawing.Size(820, 1098);
            this.groupBox1.TabIndex = 31;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Entrées";
            // 
            // gbTraitement
            // 
            this.gbTraitement.Controls.Add(this.dudTraitSel);
            this.gbTraitement.Location = new System.Drawing.Point(421, 130);
            this.gbTraitement.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.gbTraitement.Name = "gbTraitement";
            this.gbTraitement.Padding = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.gbTraitement.Size = new System.Drawing.Size(231, 92);
            this.gbTraitement.TabIndex = 24;
            this.gbTraitement.TabStop = false;
            this.gbTraitement.Text = "Type de traitement";
            // 
            // dudTraitSel
            // 
            this.dudTraitSel.Items.Add("Sans rotation");
            this.dudTraitSel.Items.Add("Avec rotation");
            this.dudTraitSel.Items.Add("Avec rotation (Pattern Matching)");
            this.dudTraitSel.Location = new System.Drawing.Point(8, 39);
            this.dudTraitSel.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.dudTraitSel.Name = "dudTraitSel";
            this.dudTraitSel.Size = new System.Drawing.Size(171, 22);
            this.dudTraitSel.TabIndex = 10;
            this.dudTraitSel.Text = "(Selection Traitement)";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Controls.Add(this.label3);
            this.groupBox2.Controls.Add(this.panel2);
            this.groupBox2.Controls.Add(this.panelRogne);
            this.groupBox2.Location = new System.Drawing.Point(845, 52);
            this.groupBox2.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Padding = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.groupBox2.Size = new System.Drawing.Size(1103, 1098);
            this.groupBox2.TabIndex = 32;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Sorties";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(401, 518);
            this.label4.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(92, 17);
            this.label4.TabIndex = 32;
            this.label4.Text = "Image verdict";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(375, 34);
            this.label3.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(148, 17);
            this.label3.TabIndex = 31;
            this.label3.Text = "Rognage automatique";
            // 
            // Form2
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoScroll = true;
            this.ClientSize = new System.Drawing.Size(1924, 1055);
            this.Controls.Add(this.bSeuillageAuto);
            this.Controls.Add(this.labelScore2);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.labelScore);
            this.Controls.Add(this.butForm1);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.groupBox2);
            this.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
            this.Name = "Form2";
            this.Text = "Projet Vision Oui-Oui";
            this.Load += new System.EventHandler(this.Form2_Load);
            ((System.ComponentModel.ISupportInitialize)(this.imageDepart)).EndInit();
            this.panel1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.imageSeuillee)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.panel2.ResumeLayout(false);
            this.panelRogne.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pbRogne)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.gbTraitement.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button butForm1;
        private System.Windows.Forms.OpenFileDialog ouvrirImage;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button buttonOuvrir;
        private System.Windows.Forms.Button button_puzzle;
        private System.Windows.Forms.PictureBox pictureBox2;
        private System.Windows.Forms.Button bSeuillageAuto;
        private System.Windows.Forms.Label labelScore2;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label labelScore;
        private System.Windows.Forms.PictureBox imageSeuillee;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Panel panelRogne;
        private System.Windows.Forms.PictureBox pbRogne;
        public System.Windows.Forms.PictureBox imageDepart;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.GroupBox gbTraitement;
        private System.Windows.Forms.DomainUpDown dudTraitSel;
    }
}