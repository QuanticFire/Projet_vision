#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <windows.h>
#include <cmath>
#include <vector>

#include "ImageCouleur.h"

#define MAGIC_NUMBER_BMP ('B'+('M'<<8)) // signature bitmap windows

// Fonction de calcul de l'�cart type d'un histogramme sous la forme d'un vector double
double stdev(std::vector<double> h)
{
	int taille = h.size();
	double moyenne = 0;
	for (int i = 0; i < taille; i++)
	{
		moyenne = moyenne + h[i];
	}
	moyenne = moyenne / taille;

	double ecart_type = 0;
	for (int i = 0; i < taille; i++)
	{
		ecart_type = ecart_type + (h[i] - moyenne)*(h[i] - moyenne);
	}

	ecart_type = ecart_type / taille;
	ecart_type = sqrt(ecart_type);
	return ecart_type;
}

// constructeurs et destructeur
CImageCouleur::CImageCouleur() {

	this->m_iHauteur  = 0;
	this->m_iLargeur  = 0;
	this->m_sNom      = "vide";
	this->m_pucData   = NULL;
	this->m_ppucPixel = NULL;
}

CImageCouleur::CImageCouleur(int hauteur, int largeur, int valR, int valV, int valB) {

	this->m_iHauteur = hauteur;
	this->m_iLargeur = largeur;
	this->m_sNom     = "inconnu";

	this->m_pucData = new unsigned char[hauteur*largeur*3];
	this->m_ppucPixel = new unsigned char*[hauteur*largeur];
	for (int i=0;i<hauteur*largeur;i++)
		this->m_ppucPixel[i] = &this->m_pucData[3*i];

	if (valR != -1) 
		for (int i=0;i<this->lireNbPixels();i++)
			this->m_ppucPixel[i][0] = valR;
	if (valV != -1) 
		for (int i=0;i<this->lireNbPixels();i++)
			this->m_ppucPixel[i][1] = valV;
	if (valB != -1) 
		for (int i=0;i<this->lireNbPixels();i++)
			this->m_ppucPixel[i][2] = valB;
}

CImageCouleur::CImageCouleur(const std::string& name) {

	BITMAPFILEHEADER header;
	BITMAPINFOHEADER infoHeader;
	
	std::ifstream f(name.c_str(),std::ios::in | std::ios::binary); // transformation d'une string en cha�ne de type C
	if (f.is_open()) {
		f.read((char*)&header,sizeof(BITMAPFILEHEADER));
		if (header.bfType != MAGIC_NUMBER_BMP) 
			throw std::string("ouverture format BMP impossible ..."); 
		else {
			f.read((char*)&infoHeader,sizeof(BITMAPINFOHEADER));
			if (infoHeader.biCompression > 0) 
				throw std::string("Format compresse non support�...");
			else {
				if (infoHeader.biBitCount == 24) {
					this->m_iHauteur = infoHeader.biHeight;
					this->m_iLargeur = infoHeader.biWidth;
					this->m_sNom.assign(name.begin(),name.end()-4);
					this->m_pucData = new unsigned char[infoHeader.biHeight*infoHeader.biWidth*3];
					this->m_ppucPixel = new unsigned char*[infoHeader.biHeight*infoHeader.biWidth];
					for (int i=0;i<infoHeader.biHeight*infoHeader.biWidth;i++)
						this->m_ppucPixel[i] = &this->m_pucData[3*i];

					// g�rer multiple de 32 bits via z�ros �ventuels ignor�s
					int complement = (((this->m_iLargeur*3-1)/4) + 1)*4 - this->m_iLargeur*3;

					for (int i= m_iHauteur-1; i >= 0; i--) {
						for (int j=0;j<m_iLargeur;j++) {
							f.read((char*)&this->m_ppucPixel[i*m_iLargeur+j][2],sizeof(char));
							f.read((char*)&this->m_ppucPixel[i*m_iLargeur+j][1],sizeof(char)); 
							f.read((char*)&this->m_ppucPixel[i*m_iLargeur+j][0],sizeof(char));
					}	

					char inutile;
					for (int k=0; k< complement; k++)
						f.read((char*)&inutile,sizeof(char));
					}
				}
				else {
					// cas d'une image en niveaux de gris
					this->m_iHauteur = infoHeader.biHeight;
					this->m_iLargeur = infoHeader.biWidth;
					this->m_sNom.assign(name.begin(),name.end()-4);
					this->m_pucData = new unsigned char[infoHeader.biHeight*infoHeader.biWidth*3];
					this->m_ppucPixel = new unsigned char*[infoHeader.biHeight*infoHeader.biWidth];
					for (int i=0;i<infoHeader.biHeight*infoHeader.biWidth;i++)
						this->m_ppucPixel[i] = &this->m_pucData[3*i];

					// lecture palette
					unsigned char* palette=NULL;
					palette = new unsigned char[256*4];
					for (int indice=0;indice<4*256;indice++) 
						f.read((char*)&palette[indice],sizeof(char));
					
					// g�rer multiple de 32 bits via z�ros �ventuels ignor�s
					int complement = (((this->m_iLargeur-1)/4) + 1)*4 - this->m_iLargeur;
					// passage du gris vers la couleur par duplication des valeurs
					for (int i= m_iHauteur-1; i >= 0; i--) {
						for (int j=0;j<m_iLargeur;j++) {
							unsigned char temp;
							f.read((char*)&temp,sizeof(char));
							this->m_ppucPixel[i*m_iLargeur+j][0]=palette[4*temp+2];
							this->m_ppucPixel[i*m_iLargeur+j][1]=palette[4*temp+1];
							this->m_ppucPixel[i*m_iLargeur+j][2]=palette[4*temp];
						}	

						char inutile;
						for (int k=0; k< complement; k++)
							f.read((char*)&inutile,sizeof(char));
					}
				}
			}
		}
		f.close();
	}
	else
		throw std::string("Ouverture impossible !");
}

CImageCouleur::CImageCouleur(const CImageCouleur& im) {

	this->m_iHauteur = im.lireHauteur();
	this->m_iLargeur = im.lireLargeur();
	this->m_sNom     = im.lireNom();
	this->m_pucData = NULL;
	this->m_ppucPixel = NULL;

	if (im.m_pucData != NULL) {
		this->m_pucData = new unsigned char[im.lireHauteur()*im.lireLargeur()*3];
		this->m_ppucPixel = new unsigned char*[im.lireHauteur()*im.lireLargeur()];
		for (int i=0;i<im.lireHauteur()*im.lireLargeur();i++)
			this->m_ppucPixel[i] = &this->m_pucData[3*i];
		
		memcpy(this->m_pucData,im.m_pucData,im.lireNbPixels()*3);
		}
}

CImageCouleur::~CImageCouleur() {

	if (this->m_ppucPixel) {
		delete[] this->m_ppucPixel;
		this->m_ppucPixel = NULL;
	}
	if (this->m_pucData) {
		delete[] this->m_pucData;
		this->m_pucData = NULL;
	}
}

void CImageCouleur::sauvegarde(const std::string& fixe) {

	BITMAPFILEHEADER header;
	BITMAPINFOHEADER infoHeader;

	if (this->m_ppucPixel) {
		std::string nomFichier = "../Res/";
		//std::string nomFichier = "C:/Temp/"; // Modification temporaire du chemin d'enregistrement pour debug projet vision IPSI3
		if (fixe.compare("") == 0)
			nomFichier += this->lireNom() + ".bmp"; // force sauvegarde dans r�pertoire Res (doit exister)
		else
			nomFichier += fixe;
		std::ofstream f(nomFichier.c_str(),std::ios::binary);
		if (f.is_open()) {
			int complement = (((this->m_iLargeur*3-1)/4) + 1)*4 - this->m_iLargeur*3;

			header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); 
			header.bfSize = header.bfOffBits + (complement+lireLargeur())*lireHauteur()*sizeof(char);
			header.bfType = MAGIC_NUMBER_BMP;
			header.bfReserved1 = 0;
			header.bfReserved2 = 0;
			f.write((char*)&header,sizeof(BITMAPFILEHEADER));
			
			infoHeader.biHeight = this->lireHauteur();
			infoHeader.biWidth = this->lireLargeur();
			infoHeader.biCompression = 0;
			infoHeader.biClrUsed = 0;
			infoHeader.biBitCount = 24;
			infoHeader.biSizeImage = 0; //pas de compression;
			infoHeader.biClrUsed = 0;
			infoHeader.biClrImportant = 0;
			infoHeader.biHeight = this->lireHauteur();
			infoHeader.biWidth = this->lireLargeur();
			infoHeader.biPlanes = 1;
			infoHeader.biSize = sizeof(infoHeader);
			infoHeader.biSizeImage = this->lireNbPixels();
			f.write((char*)&infoHeader,sizeof(BITMAPINFOHEADER));

			for (int i= this->lireHauteur()-1; i >= 0; i--) {
				for (int j=0;j<this->lireLargeur();j++) {
					f.write((char*)&this->m_ppucPixel[i*this->lireLargeur()+j][2],sizeof(char));
					f.write((char*)&this->m_ppucPixel[i*this->lireLargeur()+j][1],sizeof(char)); 
					f.write((char*)&this->m_ppucPixel[i*this->lireLargeur()+j][0],sizeof(char));
				}	
				// g�rer multiple de 32 bits
				char inutile;
				for (int k=0; k< complement; k++)
					f.write((char*)&inutile,sizeof(char)); 
			}
		f.close();
		}
		else
			throw std::string("Impossible de creer le fichier de sauvegarde !");
	}
	else
		throw std::string("Pas de donnee a sauvegarder !");
}

CImageCouleur& CImageCouleur::operator=(const CImageCouleur& im) {

	if (&im == this)
		return *this;

	this->m_iHauteur = im.lireHauteur();
	this->m_iLargeur = im.lireLargeur();
	this->m_sNom     = im.lireNom();

	if (this->m_pucData) 
		delete[] this->m_pucData;
	if (this->m_ppucPixel) 
		delete[] this->m_ppucPixel;

	this->m_pucData = new unsigned char[im.lireHauteur()*im.lireLargeur()*3];
	this->m_ppucPixel = new unsigned char*[im.lireHauteur()*im.lireLargeur()];
	for (int i=0;i<im.lireHauteur()*im.lireLargeur();i++)
		this->m_ppucPixel[i] = &this->m_pucData[3*i];

	memcpy(this->m_pucData,im.m_pucData,im.lireNbPixels()*3);

return *this;
}

// histogramme 

std::vector<std::vector<unsigned long>> CImageCouleur::histogramme(bool enregistrementCSV, int pas) {

	std::vector<std::vector<unsigned long>> h;

	h.resize(3); // 3 plans 
	for (int plan=0;plan<3;plan++) 
		h[plan].resize(256/pas,0);

	for (int i=0;i<this->lireNbPixels();i++) { 
		h[0][this->operator()(i)[0]/pas] += 1L;
		h[1][this->operator()(i)[1]/pas] += 1L;
		h[2][this->operator()(i)[2]/pas] += 1L;
	}
	if (enregistrementCSV) {
		std::string fichier = "../Res/" + this->lireNom() + ".csv";
		std::ofstream f (fichier.c_str());

		if (!f.is_open())
			std::cout << "Impossible d'ouvrir le fichier en ecriture !" << std::endl;
		else {
			for (int i=0;i<(int)h[0].size();i++)
				f << h[0][i] << " ; " << h[1][i] << " ; " << h[2][i] << " ; " << std::endl;
		}
		f.close();
	}

	return h;
}


// gestion des plans
CImageNdg CImageCouleur::plan(int choix, float poidsRouge, float poidsVert, float poidsBleu) { // 0 -> R ou H, 1 -> V ou S, 2 -> B ou V et 3 -> luminance d'o� les poids fct de l'illuminant
	
	CImageNdg out(this->lireHauteur(),this->lireLargeur());
	std::stringstream convert;
	convert << choix;
	out.ecrireNom(this->lireNom() + convert.str());
	out.choixPalette("grise"); // par d�faut
	out.ecrireBinaire(false);

	switch(choix) {
	case 0 :
		for (int i=0;i<this->lireNbPixels();i++)
			out(i) = this->operator()(i)[0];
		break;
	case 1 :
		for (int i=0;i<this->lireNbPixels();i++)
			out(i) = this->operator()(i)[1];
		break;
	case 2 : 
		for (int i=0;i<this->lireNbPixels();i++)
			out(i) = this->operator()(i)[2];
		break;
	case 3: 
		for (int i=0;i<this->lireNbPixels();i++)
			out(i) = (unsigned char)(poidsRouge*(float)this->operator()(i)[0] + poidsVert*(float)this->operator()(i)[1] + poidsBleu*(float)this->operator()(i)[2]);
	}
	return out;
}

CImageCouleur& CImageCouleur::miseAJourPlan(int choix, const CImageNdg& plan) {

	for (int i=0;i<this->lireNbPixels();i++)
		this->operator()(i)[choix] = plan(i);

		return *this;
}


// conversion espace HSV sur [0-255] pour chaque plan

CImageCouleur CImageCouleur::conversion(const std::string& methode) const {

	CImageCouleur out(this->lireHauteur(),this->lireLargeur(),0,0,0);
	out.ecrireNom(this->lireNom() + "HSV");

	if (methode.compare("HSV") == 0) {
		unsigned char rgb_min, rgb_max;	

		for (int i=0;i<this->lireNbPixels();i++) 
		{
			rgb_min = MIN3(this->operator()(i)[0], this->operator()(i)[1], this->operator()(i)[2]);
			rgb_max = MAX3(this->operator()(i)[0], this->operator()(i)[1], this->operator()(i)[2]);

			if (rgb_max != rgb_min)
			{
				if (rgb_max == this->operator()(i)[0]) {
					out(i)[0] = 0 + 43*(this->operator()(i)[1] - this->operator()(i)[2])/(rgb_max - rgb_min);
				}
				else 
					if (rgb_max == this->operator()(i)[1]) {
						out(i)[0] = 85 + 43*(this->operator()(i)[2] - this->operator()(i)[0])/(rgb_max - rgb_min);
					}
					else  {
						out(i)[0] = 171 + 43*(this->operator()(i)[0] - this->operator()(i)[1])/(rgb_max - rgb_min);
					}
			}
			if (rgb_max != 0)
				out(i)[1] = rgb_max-rgb_min;
			out(i)[2] = rgb_max;
		}
	}
	return out;
}

CImageCouleur CImageCouleur::transformation(const std::string& methode, int vMin, int vMax) {

	CImageCouleur out(this->lireHauteur(),this->lireLargeur());
	out.m_sNom     = this->lireNom()+"T";

	if (methode.compare("expansion") == 0) {
		CImageNdg pR = this->plan(0);
		CImageNdg pG = this->plan(1);
		CImageNdg pB = this->plan(2);

		pR = pR.transformation("expansion",vMin,vMax);
		pG = pG.transformation("expansion",vMin,vMax);
		pB = pB.transformation("expansion",vMin,vMax);

		out.miseAJourPlan(0,pR);
		out.miseAJourPlan(1,pG);
		out.miseAJourPlan(2,pB);
		}
		else
			if (methode.compare("egalisation") == 0) {
				CImageNdg pR = this->plan(0);
				CImageNdg pG = this->plan(1);
				CImageNdg pB = this->plan(2);

				pR = pR.transformation("egalisation");
				pG = pG.transformation("egalisation");
				pB = pB.transformation("egalisation");

				out.miseAJourPlan(0,pR);
				out.miseAJourPlan(1,pG);
				out.miseAJourPlan(2,pB);
			}

	return out;
}

std::vector<MOMENTS> CImageCouleur::signatures()
{
	std::vector<MOMENTS> mts;

	mts.resize(3);

	for (int plan = 0; plan < 3; plan++)
	{
		CImageNdg p = this->plan(plan);

		std::vector<unsigned long> hist;
		hist = p.histogramme();

		mts.at(plan) = p.signatures(hist);
	}

	return mts;
}

CImageCouleur CImageCouleur::detection_piece(CImageCouleur piece)
{
	// Cr�ation de l'image de sortie � partir de l'objet point� (image de r�f�rence)
	CImageCouleur ref(this->lireHauteur(), this->lireLargeur());
	for (int i = 0; i < ref.lireHauteur(); i++)
	{
		for (int j = 0; j < ref.lireLargeur(); j++)
		{
			ref(i, j)[0] = this->operator()(i, j)[0];
			ref(i, j)[1] = this->operator()(i, j)[1];
			ref(i, j)[2] = this->operator()(i, j)[2];
		}
	}

	// S�paration des plans de couleur
	CImageNdg ref_plans[3];
	ref_plans[0] = ref.plan(1);
	ref_plans[1] = ref.plan(2);
	ref_plans[2] = ref.plan(3);

	// Initialisations
	int ref_hauteur = ref.lireHauteur();
	int ref_largeur = ref.lireLargeur();
	int nb_piece_hauteur = 5;
	int nb_piece_largeur = 9;
	int pas_hauteur = int(floor(ref_hauteur / nb_piece_hauteur));
	int pas_largeur = int(floor(ref_largeur / nb_piece_largeur));

	int imagette_hauteur = int(floor(pas_hauteur / 2));
	int imagette_largeur = int(floor(pas_largeur / 2));

	// Cr�ation d'un tableau d'images noires qui vont acceuillir les quarts d'images des trois plans de la pi�ce
	CImageNdg pieces_ref[3][5][9][2][2];
	for (int plan = 0; plan < 3; plan++)
	{
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				for (int v = 0; v < 2; v++)
				{
					for (int w = 0; w < 2; w++)
					{
						pieces_ref[plan][i][j][v][w] = CImageNdg(imagette_hauteur, imagette_largeur, 0);
					}
				}
			}
		}
	}


	// Parcours sur chaque plan
	for (int plan = 0; plan < 3; plan++)
	{
		// Parcours sur chaque "pieces"
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				// Parcours sur chaque imagette de chaque piece
				for (int v = 0; v < 2; v++)
				{
					for (int w = 0; w < 2; w++)
					{
						int hauteur_depart = i*pas_hauteur + v*imagette_hauteur;
						int largeur_depart = j*pas_largeur + w*imagette_largeur;
						int hauteur_fin = i*pas_hauteur + (v + 1)*imagette_hauteur;
						int largeur_fin = j*pas_largeur + (w + 1)*imagette_largeur;

						// Parcours sur chaque pixel de chaque imagette
						for (int x = hauteur_depart; x < hauteur_fin - 1; x++)
						{
							for (int y = largeur_depart; y < largeur_fin - 1; y++)
							{
								//pieces_ref[plan][i][j](x, y) = ref_plans[plan](x, y);
								pieces_ref[plan][i][j][v][w](x - hauteur_depart, y - largeur_depart) = ref_plans[plan](x, y);
							}
						}
					}
				}

			}
		}
	}

	/////////////////////////////////////// COMPARAISON AVEC LES REFERENCES ////////////////////////////////////

	// Extraction des trois plans de la pi�ce
	CImageNdg piece1 = piece.plan(1);
	CImageNdg piece2 = piece.plan(2);
	CImageNdg piece3 = piece.plan(3);

	int hauteur = piece1.lireHauteur();
	int largeur = piece1.lireLargeur();

	// Cr�ation d'un tableau d'images noires qui vont acceuillir les quarts d'images des trois plans de la pi�ce
	CImageNdg piece_array[3][4];
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			piece_array[j][i] = CImageNdg(hauteur / 2, largeur / 2, 0);
		}
	}

	int hauteur_sur_2 = int(floor(hauteur / 2));
	int largeur_sur_2 = int(floor(largeur / 2));

	// On rempli les imagettes
	for (int i = 0; i < hauteur_sur_2; i++)
	{
		for (int j = 0; j < largeur_sur_2; j++)
		{
			piece_array[0][0](i, j) = piece1(i, j);
			piece_array[1][0](i, j) = piece2(i, j);
			piece_array[2][0](i, j) = piece3(i, j);
		}
	}

	for (int i = 0; i < hauteur_sur_2; i++)
	{
		for (int j = largeur_sur_2; j < largeur - 1; j++)
		{
			piece_array[0][1](i, j - largeur_sur_2) = piece1(i, j);
			piece_array[1][1](i, j - largeur_sur_2) = piece2(i, j);
			piece_array[2][1](i, j - largeur_sur_2) = piece3(i, j);
		}
	}

	for (int i = hauteur_sur_2; i < hauteur - 1; i++)
	{
		for (int j = 0; j < largeur_sur_2; j++)
		{
			piece_array[0][2](i - hauteur_sur_2, j) = piece1(i, j);
			piece_array[1][2](i - hauteur_sur_2, j) = piece2(i, j);
			piece_array[2][2](i - hauteur_sur_2, j) = piece3(i, j);
		}
	}

	for (int i = hauteur_sur_2; i < hauteur - 1; i++)
	{
		for (int j = largeur_sur_2; j < largeur - 1; j++)
		{
			piece_array[0][3](i - hauteur_sur_2, j - largeur_sur_2) = piece1(i, j);
			piece_array[1][3](i - hauteur_sur_2, j - largeur_sur_2) = piece1(i, j);
			piece_array[2][3](i - hauteur_sur_2, j - largeur_sur_2) = piece1(i, j);
		}
	}

	// On se retrouve avec :
	// Un tableau des pi�ces de r�f�rence : CImageNdg pieces_ref[3][5][9][2][2]
	// Un tableau contenant les trois plans des 4 imagettes de la pi�ece test : CImageNdg piece_array[3][4]

	// D�marrage des op�rations de calcul de distance d'histogramme

	// Calcul histogramme piece test
	std::vector<double> P0_I0 = piece_array[0][0].histogramme_norm();
	std::vector<double> P0_I1 = piece_array[0][1].histogramme_norm();
	std::vector<double> P0_I2 = piece_array[0][2].histogramme_norm();
	std::vector<double> P0_I3 = piece_array[0][3].histogramme_norm();

	std::vector<double> P1_I0 = piece_array[1][0].histogramme_norm();
	std::vector<double> P1_I1 = piece_array[1][1].histogramme_norm();
	std::vector<double> P1_I2 = piece_array[1][2].histogramme_norm();
	std::vector<double> P1_I3 = piece_array[1][3].histogramme_norm();

	std::vector<double> P2_I0 = piece_array[2][0].histogramme_norm();
	std::vector<double> P2_I1 = piece_array[2][1].histogramme_norm();
	std::vector<double> P2_I2 = piece_array[2][2].histogramme_norm();
	std::vector<double> P2_I3 = piece_array[2][3].histogramme_norm();

	// Initialisation tableau des distances d'histogramme
	double distances[45];
	for (int i = 0; i < 45; i++)
	{
		distances[i] = 0;
	}

	// Initialisation tableau des �carts types d'histogramme
	double stdev_histo[45];

	// Initialisation d'un tableau contenant la distance totale
	double total_dist[45];

	// boucle sur chaque piece de r�f�rence
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 9; j++)
		{

			// Calcul des histogrammes des imagettes de reference
			std::vector<double> Ref_P0_I0 = pieces_ref[0][i][j][0][0].histogramme_norm();
			std::vector<double> Ref_P0_I1 = pieces_ref[0][i][j][0][1].histogramme_norm();
			std::vector<double> Ref_P0_I2 = pieces_ref[0][i][j][1][0].histogramme_norm();
			std::vector<double> Ref_P0_I3 = pieces_ref[0][i][j][1][1].histogramme_norm();

			std::vector<double> Ref_P1_I0 = pieces_ref[1][i][j][0][0].histogramme_norm();
			std::vector<double> Ref_P1_I1 = pieces_ref[1][i][j][0][1].histogramme_norm();
			std::vector<double> Ref_P1_I2 = pieces_ref[1][i][j][1][0].histogramme_norm();
			std::vector<double> Ref_P1_I3 = pieces_ref[1][i][j][1][1].histogramme_norm();

			std::vector<double> Ref_P2_I0 = pieces_ref[2][i][j][0][0].histogramme_norm();
			std::vector<double> Ref_P2_I1 = pieces_ref[2][i][j][0][1].histogramme_norm();
			std::vector<double> Ref_P2_I2 = pieces_ref[2][i][j][1][0].histogramme_norm();
			std::vector<double> Ref_P2_I3 = pieces_ref[2][i][j][1][1].histogramme_norm();

			// Calcul de la distance d'histogramme
			for (int z = 0; z < 256; z++)
			{
				distances[i * 9 + j] = distances[i * 9 + j] + abs(P0_I0[z] - Ref_P0_I0[z]) + abs(P0_I1[z] - Ref_P0_I1[z]) + abs(P0_I2[z] - Ref_P0_I2[z]) + abs(P0_I3[z] - Ref_P0_I3[z]) + abs(P1_I0[z] - Ref_P1_I0[z]) + abs(P1_I1[z] - Ref_P1_I1[z]) + abs(P1_I2[z] - Ref_P1_I2[z]) + abs(P1_I3[z] - Ref_P1_I3[z]) + abs(P2_I0[z] - Ref_P2_I0[z]) + abs(P2_I1[z] - Ref_P2_I1[z]) + abs(P2_I2[z] - Ref_P2_I2[z]) + abs(P2_I3[z] - Ref_P2_I3[z]);
			}

			// Calcul de la distance par ecart type (69% de d�tection en solo)
			stdev_histo[i * 9 + j] = abs(stdev(P0_I0) - stdev(Ref_P0_I0)) + abs(stdev(P0_I1) - stdev(Ref_P0_I1)) + abs(stdev(P0_I2) - stdev(Ref_P0_I2)) + abs(stdev(P0_I3) - stdev(Ref_P0_I3)) + abs(stdev(P1_I0) - stdev(Ref_P1_I0)) + abs(stdev(P1_I1) - stdev(Ref_P1_I1)) + abs(stdev(P1_I2) - stdev(Ref_P1_I2)) + abs(stdev(P1_I3) - stdev(Ref_P1_I3)) + abs(stdev(P2_I0) - stdev(Ref_P2_I0)) + abs(stdev(P2_I1) - stdev(Ref_P2_I1)) + abs(stdev(P2_I2) - stdev(Ref_P2_I2)) + abs(stdev(P2_I3) - stdev(Ref_P2_I3));

			// Calcul de la distance totale
			// 89% de d�tection avec 0.007
			total_dist[i * 9 + j] = stdev_histo[i * 9 + j] + 0.005 * distances[i * 9 + j];
			//cout << "dist stdev : " << stdev_histo[i * 9 + j] << '\n';
			//cout << "dist histo : " << distances[i * 9 + j] << '\n';
			//cout << "dist tot : " << total_dist[i * 9 + j] << '\n';
		}
	}

	double distance_min = 999999;
	int index_distance_min = -1;
	for (int i = 0; i < 45; i++)
	{
		if (total_dist[i] < distance_min)
		{
			distance_min = total_dist[i];
			index_distance_min = i;
		}
	}

	//cout << " verdict : " << index_distance_min + 1;

	int pos_hauteur = int(floor(index_distance_min / 9));
	int pos_largeur = (index_distance_min) % 9;

	// Cr�ation d'un cadre rouge sur l'image au niveau de la pi�ce d�tect�e
	for (int i = pos_hauteur*pas_hauteur; i < (pos_hauteur + 1)*pas_hauteur; i++)
	{
		// C�t� gauche
		for (int shift = 1; shift < 10; shift++)
		{
			ref(i, pos_largeur*pas_largeur + shift)[0] = 255;
			ref(i, pos_largeur*pas_largeur + shift)[1] = 0;
			ref(i, pos_largeur*pas_largeur + shift)[2] = 0;
		}

		// C�t� droit
		for (int shift = 1; shift < 10; shift++)
		{
			ref(i, (pos_largeur + 1)*pas_largeur - shift)[0] = 255;
			ref(i, (pos_largeur + 1)*pas_largeur - shift)[1] = 0;
			ref(i, (pos_largeur + 1)*pas_largeur - shift)[2] = 0;
		}
	}
	for (int i = pos_largeur*pas_largeur; i < (pos_largeur + 1)*pas_largeur; i++)
	{
		// C�t� haut
		for (int shift = 1; shift < 10; shift++)
		{
			ref(pos_hauteur*pas_hauteur + shift, i)[0] = 255;
			ref(pos_hauteur*pas_hauteur + shift, i)[1] = 0;
			ref(pos_hauteur*pas_hauteur + shift, i)[2] = 0;
		}

		// C�t� bas
		for (int shift = 1; shift < 10; shift++)
		{
			ref((pos_hauteur + 1)*pas_hauteur - shift, i)[0] = 255;
			ref((pos_hauteur + 1)*pas_hauteur - shift, i)[1] = 0;
			ref((pos_hauteur + 1)*pas_hauteur - shift, i)[2] = 0;
		}
	}

	return ref;
}