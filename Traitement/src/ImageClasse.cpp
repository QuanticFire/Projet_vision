#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <windows.h>
#include <cmath>
#include <vector>
#include <ctime>
#include <stack>

#include "ImageClasse.h"

#define MAGIC_NUMBER_BMP ('B'+('M'<<8)) // signature bitmap windows

// constructeurs et destructeur
CImageClasse::CImageClasse() {

	this->m_iHauteur   = 0;
	this->m_iLargeur   = 0;
	this->m_sNom       = "vide";
	this->m_lNbRegions = 0;     
	this->m_pucPixel   = NULL;
}

CImageClasse::CImageClasse(int hauteur, int largeur) {

	this->m_iHauteur   = hauteur;
	this->m_iLargeur   = largeur;
	this->m_sNom       = "inconnu";
	this->m_lNbRegions = 0;
	this->m_pucPixel   = new unsigned long[hauteur*largeur];
	for (int i=0;i<this->lireNbPixels();i++)
		this->m_pucPixel[i] = 0; 
}

CImageClasse::CImageClasse(const CImageNdg& im, std::string choix) {

	this->m_iHauteur   = im.lireHauteur();
	this->m_iLargeur   = im.lireLargeur();
	this->m_sNom       = im.lireNom()+"E";
	this->m_lNbRegions = 0;
	this->m_pucPixel   = new unsigned long[m_iHauteur*m_iLargeur];


	CImageClasse withBords(im.lireHauteur()+2,im.lireLargeur()+2); 
	int i,ii,j,jj;

    for (i=0,ii=1; i < im.lireHauteur(); i++, ii++) // pure copie 
      for (j=0,jj=1; j < im.lireLargeur(); j++, jj++)
		  withBords(ii,jj) = (unsigned long)im(i,j);

	int* tableEtiquette = NULL;

	int k=1;
	int eps;
	int* predNonNul = new int [6];
	int nbPredNonNuls;

	int nbMaxEtiquettes = im.lireHauteur()*im.lireLargeur();

	tableEtiquette = new int [nbMaxEtiquettes];
	for (int kk=0;kk<nbMaxEtiquettes;kk++) {
		tableEtiquette[kk]= kk;
	}

	if (choix.compare("V4") == 0) {
		for (i=1;i<withBords.lireHauteur()-1;i++)
			for (j=1;j<withBords.lireLargeur()-1;j++) {
				if (withBords(i,j) != 0) {
				// V4 
				int pred1 = withBords(i,j-1);
				int pred3 = withBords(i-1,j);
			
				nbPredNonNuls = 0;
				if (pred1) predNonNul[nbPredNonNuls++] = pred1; 
				if (pred3) predNonNul[nbPredNonNuls++] = pred3; 
		
				if (nbPredNonNuls == 0) {
					withBords(i,j) = k;
					k++;
				}
				else {
					eps = 0;
					int minPred = tableEtiquette[predNonNul[0]];
					int maxPred = tableEtiquette[predNonNul[0]];
					while (eps<nbPredNonNuls) {
						if (tableEtiquette[predNonNul[eps]] < minPred) minPred = tableEtiquette[predNonNul[eps]];
						if (tableEtiquette[predNonNul[eps]] > maxPred) maxPred = tableEtiquette[predNonNul[eps]];
						eps ++;
					}
				if (minPred == maxPred) 
					withBords(i,j) = minPred;
				else {
					withBords(i,j) = minPred;
					for (eps=0;eps<nbPredNonNuls;eps++) {
						int beta = predNonNul[eps];
						while (tableEtiquette[beta] != minPred) {
							int m = tableEtiquette[beta];
							tableEtiquette[beta] = minPred;
							beta = m;
						}
					}
				}
				}
				}
			}
	}
	else { // V8
		for (i=1;i<withBords.lireHauteur()-1;i++)
			for (j=1;j<withBords.lireLargeur()-1;j++) {
				if (withBords(i,j) != 0) {
					// V8 
					int pred1 = withBords(i,j-1);
					int pred2 = withBords(i-1,j-1);
					int pred3 = withBords(i-1,j);
					int pred4 = withBords(i-1,j+1);
		
					nbPredNonNuls = 0;
					if (pred1) predNonNul[nbPredNonNuls++] = pred1; 
					if (pred2) predNonNul[nbPredNonNuls++] = pred2;
					if (pred3) predNonNul[nbPredNonNuls++] = pred3; 
					if (pred4) predNonNul[nbPredNonNuls++] = pred4;

					if (nbPredNonNuls == 0) {
						withBords(i,j) = k;
						k++;
					}
					else {
						eps = 0;
						int minPred = tableEtiquette[predNonNul[0]];
						int maxPred = tableEtiquette[predNonNul[0]];
						while (eps<nbPredNonNuls) {
							if (tableEtiquette[predNonNul[eps]] < minPred) minPred = tableEtiquette[predNonNul[eps]];
							if (tableEtiquette[predNonNul[eps]] > maxPred) maxPred = tableEtiquette[predNonNul[eps]];
							eps ++;
						}
					if (minPred == maxPred) 
						withBords(i,j) = minPred;
					else {
						withBords(i,j) = minPred;
						for (eps=0;eps<nbPredNonNuls;eps++) {
							int beta = predNonNul[eps];
							while (tableEtiquette[beta] != minPred) {
								int m = tableEtiquette[beta];
								tableEtiquette[beta] = minPred;
								beta = m;
							}
						}
					}
				}
			}
		}	
	} 
  
	// actualisation de la table d'équivalence
	for (int kk=1;kk<k;kk++) {
		int m = kk;
		while (tableEtiquette[m] != m) 
		m = tableEtiquette[m];
		tableEtiquette[kk] = m;
	}

	// on vire les trous histoire que nbObjets = max de l'image label
	int* etiquettes = new int [k+1];
	for (int kk=0;kk<k;kk++)
		etiquettes[kk] = 0;

	// histo pour repérer les trous
	for (int kk=1;kk<k;kk++)
		etiquettes[tableEtiquette[kk]]++;

	// on remet à jour les index (etiquetage définitif hors trou)
	etiquettes[0] = 0;
	int compt = 1;
	for (int kk=1;kk<k;kk++) {
		if (etiquettes[kk]) 
		etiquettes[kk] = compt++;
	}

	for (i=0;i<withBords.lireHauteur();i++)
		for (j=0;j<withBords.lireLargeur();j++) {
			withBords(i,j) = etiquettes[tableEtiquette[withBords(i,j)]];
		}

	delete [] etiquettes;
	delete [] tableEtiquette;

	// création image labélisée
	// suppression des 2 lignes et 2 colonnes artificiellement créées

	for (i=0,ii=1; i < this->lireHauteur(); i++, ii++)  
		for (j=0,jj=1; j < this->lireLargeur(); j++, jj++)
			this->operator()(i,j) = withBords(ii,jj);

	m_lNbRegions = compt-1;
	std::cout << compt-1 << " objets dans l'image..." << std::endl;
}


CImageClasse::CImageClasse(const CImageClasse& in, std::string choix, std::string voisinage) {

	if (choix.compare("sans") == 0) { // simple copie

		this->m_iHauteur   = in.lireHauteur();
		this->m_iLargeur   = in.lireLargeur();
		this->m_sNom       = in.lireNom();
		this->m_lNbRegions = in.lireNbRegions();
		this->m_pucPixel = new unsigned long[in.lireHauteur()*in.lireLargeur()];
		if (in.m_pucPixel != NULL)
			memcpy(this->m_pucPixel,in.m_pucPixel,in.lireNbPixels()*sizeof(unsigned long));
	}
	else { 
		// détection cas objets/fond ou ND, présence ou non d'une classe à 0

		bool objetsFond = false;
		int pix=0;
		while ( (pix < in.lireNbPixels()) && (!objetsFond) ) {
			if (in(pix) == 0) objetsFond = true;
			pix++;
		}

		if (objetsFond) { // cas présence fond à 0

			CImageNdg im(in.lireHauteur(),in.lireLargeur());
			for (int i=0;i<in.lireNbPixels();i++)
				im(i) = (in(i) > 0) ? 1 : 0;
			CImageClasse out(im,voisinage);

			this->m_sNom       = out.lireNom()+"RE";
			this->m_lNbRegions = out.lireNbRegions();
			this->m_pucPixel   = new unsigned long[m_iHauteur*m_iLargeur];

			memcpy(this->m_pucPixel,out.m_pucPixel,out.lireNbPixels()*sizeof(unsigned long));
		}
		else { // cas nuées dynamiques donc ré-étiquetage classe après classe
			this->m_iHauteur   = in.lireHauteur();
			this->m_iLargeur   = in.lireLargeur();
			this->m_sNom       = in.lireNom()+"RE";
			this->m_lNbRegions = 0;

			this->m_pucPixel = new unsigned long[in.lireHauteur()*in.lireLargeur()];
			for (int pix=0;pix<this->lireNbPixels();pix++)
				this->m_pucPixel[pix] = 0;

			for (int classe = 1;classe <= in.lireNbRegions(); classe++) {			
				CImageNdg im(in.lireHauteur(),in.lireLargeur());

				for (int i=0;i<in.lireNbPixels();i++)
					im(i) = (in(i) == classe) ? 1 : 0;
				CImageClasse inter(im,voisinage);
				for (int pix=0;pix<inter.lireNbPixels();pix++)
					if (inter(pix))
						this->operator()(pix) = inter(pix)+this->lireNbRegions(); 

				this->ecrireNbRegions(this->lireNbRegions()+inter.lireNbRegions());
			}
		std::cout << this->lireNbRegions() << " nouvelles régions connexes dans l'image..." << std::endl;
		}
	}
}

CImageClasse::CImageClasse(const CImageNdg& im, int nbClusters, std::string choix) {

	this->m_iHauteur   = im.lireHauteur();
	this->m_iLargeur   = im.lireLargeur();
	this->m_sNom       = im.lireNom()+"ND";
	this->m_lNbRegions = 0;
	this->m_pucPixel   = new unsigned long[m_iHauteur*m_iLargeur];


	// contiendra la somme des elements de la classe
	// pour pouvoir calculer le centre de gravité !
	std::vector<unsigned long> sommeClasses;

	// contiendra le nombre des elements de la classe
	// pour pouvoir calculer le centre de gravité !
	std::vector<unsigned long> nbEltsClasses;

	// contiendra le centre de chaque classe
	std::vector<unsigned char> indexClasses;

	// contiendra la classe de chaque niveau de gris
	// imageFinale = lut[imageEntree]
	std::vector<int> lut;

	// allocation de k+1 classes
	// volonté d'affecter l'indice i à la classe i
	nbEltsClasses.resize(nbClusters+1); 
	for (int i=1;i<=nbClusters;i++)
		nbEltsClasses[i]=0;
	sommeClasses.resize(nbClusters+1);
	for (int i=1;i<=nbClusters;i++)
		sommeClasses[i]=0L;

	// moyenne image et histo pour initialisation
	std::vector<unsigned long> h;
	h.resize(256,0);
	for (int i=0;i<this->lireNbPixels();i++) 
		h[im(i)] += 1L;
	float mean=0;
	for (int i=0;i<256;i++) 
		mean += h[i]*i;
	mean /= this->lireNbPixels();

	// nb de niveaux de gris dans image initiale
	// si k supérieur, alors on diminue le nb de classes en sortie : nbClasses
	int nbLevels=0;
	for (int i=0;i<256;i++)
		if (h[i]) nbLevels++;
	int nbClasses;
	nbClasses=min(nbClusters,nbLevels);
	m_lNbRegions = nbClasses;


	if (strcmp(choix.c_str(),"aleatoire") == 0) { // tirage aléatoire des germes initiaux 
		srand((unsigned)time( NULL ));	
		indexClasses.resize(nbClasses+1);

		int num=1;
		while (num <= nbClasses) {
			int encours = rand()%256;
			bool dejaPris=false;
			for (int j=1;j<num;j++)
				if (indexClasses[j] == encours) dejaPris=true;
			if (dejaPris==false) {
				indexClasses[num] = encours;
				num++;
			}
		}
	}
	else if (strcmp(choix.c_str(),"regulier") == 0) { // intervalles réguliers entre germes 
		indexClasses.resize(nbClasses+1);
		// recherche du min et du max
		int min = 0, max = 255;
		while (h[min] == 0)
			min++;
		while (h[max] == 0)
			max--;

		int num=1;
		int encours = min;
		int pas = (max-min)/nbClasses;

		while (num <= nbClasses) {  
			indexClasses[num] = encours;
			encours += pas;
			num++;
		}
	}

	std::cout << "classification en " << nbClasses << " classes..." << std::endl;

	lut.resize(256);
	int nbTours = 0 ;
	bool change = true ;

	while ( (!nbTours) ||  ((change) && ( nbTours < MAX_ITER))) {
		// Remise à zéro à chaque tour 
		for (int i=1;i<=nbClasses;i++) {
			nbEltsClasses[i]=0;
			sommeClasses[i]=0L;
		}
		change = false ;

		// Pour chaque point trouver son plus proche
		for (int i =0; i < 256 ; i++)
		{    
		  unsigned char encours = i;
		  int plusproche = 1 ;
		  for (int j = 2; j <= nbClasses ; j++)
			{              
			  if (EUCLIDEAN(encours, indexClasses[j]) < EUCLIDEAN(encours, indexClasses[plusproche]))
				plusproche = j ;
			}        
		  lut[i] = plusproche ;
		  nbEltsClasses[plusproche] += h[encours] ;
		  sommeClasses[plusproche] += h[encours]*encours ;
		}

	  // On recalcule le centre de chaque classe
	  // a partir des nouveaux germes.
	  for (int j = 1 ; j <= nbClasses ; j++)
		{
		  if (nbEltsClasses[j])
			  sommeClasses[j] /= nbEltsClasses[j] ;
		  else
			  sommeClasses[j] = (unsigned long)mean;
		  if (EUCLIDEAN(indexClasses[j],sommeClasses[j]) > 0.001)  
			  change = true ;
		  indexClasses[j] = (unsigned char)sommeClasses[j] ;
		}
	  nbTours ++ ;
	  std::cout << "Iteration : " << nbTours <<    std::endl ;     


	}

	// on remplace chaque pixel par sa classe
	for (int i=0; i < this->lireNbPixels(); i++) 
		this->operator()(i) = lut[im(i)]; 
}


CImageClasse::CImageClasse(const CImageCouleur& im, int nbClusters, std::string choix, int plan) {

	this->m_iHauteur   = im.lireHauteur();
	this->m_iLargeur   = im.lireLargeur();
	this->m_sNom       = im.lireNom()+"ND";
	this->m_lNbRegions = 0;
	this->m_pucPixel   = new unsigned long[m_iHauteur*m_iLargeur];


	// contiendra la somme des elements de la classe
	// pour pouvoir calculer le centre de gravité !
	std::vector<unsigned long> sommeClasses;

	// contiendra le nombre des elements de la classe
	// pour pouvoir calculer le centre de gravité !
	std::vector<unsigned long> nbEltsClasses;

	// contiendra le centre de chaque classe
	std::vector<unsigned char> indexClasses;

	// contiendra la classe de chaque niveau de gris
	// imageFinale = lut[imageEntree]
	std::vector<int> lut;

	// allocation de k+1 classes
	// volonté d'affecter l'indice i à la classe i
	nbEltsClasses.resize(nbClusters+1); 
	for (int i=1;i<=nbClusters;i++)
		nbEltsClasses[i]=0;
	sommeClasses.resize(nbClusters+1);
	for (int i=1;i<=nbClusters;i++)
		sommeClasses[i]=0L;

	// extraction plan H pour optimisation basée histogramme 

	CImageCouleur hsv = im.conversion("HSV");

	// moyenne sur plan Hue et histo pour initialisation 
	std::vector<unsigned long> h;
	h.resize(256,0);
	for (int i=0;i<this->lireNbPixels();i++) 
		h[hsv(i)[plan]] += 1L;
	float mean=0;
	for (int i=0;i<256;i++) 
		mean += h[i]*i;
	mean /= this->lireNbPixels();

	// nb de niveaux de gris dans image initiale
	// si k supérieur, alors on diminue le nb de classes en sortie : nbClasses
	int nbLevels=0;
	for (int i=0;i<256;i++)
		if (h[i]) nbLevels++;
	int nbClasses;
	nbClasses=min(nbClusters,nbLevels);
	m_lNbRegions = nbClasses;

	if (strcmp(choix.c_str(),"aleatoire") == 0) { // tirage aléatoire des germes initiaux 
		srand((unsigned)time( NULL ));	
		indexClasses.resize(nbClasses+1);

		int num=1;
		while (num <= nbClasses) {
			int encours = rand()%256;
			bool dejaPris=false;
			for (int j=1;j<num;j++)
				if (indexClasses[j] == encours) dejaPris=true;
			if (dejaPris==false) {
				indexClasses[num] = encours;
				num++;
			}
		}
	}
	else if (strcmp(choix.c_str(),"regulier") == 0) { // intervalles réguliers entre germes 
		indexClasses.resize(nbClasses+1);
		// recherche du min et du max
		int min = 0, max = 255;
		while (h[min] == 0)
			min++;
		while (h[max] == 0)
			max--;

		int num=1;
		int encours = min;
		int pas = (max-min)/nbClasses;

		while (num <= nbClasses) {  
			indexClasses[num] = encours;
			encours += pas;
			num++;
		}
	}

	std::cout << "classification en " << nbClasses << " classes..." << std::endl;

	lut.resize(256);
	int nbTours = 0 ;
	bool change = true ;

	while ( (!nbTours) ||  ((change) && ( nbTours < MAX_ITER))) {
		// Remise à zéro à chaque tour 
		for (int i=1;i<=nbClasses;i++) {
			nbEltsClasses[i]=0;
			sommeClasses[i]=0L;
		}
		change = false ;

		// Pour chaque point trouver son plus proche
		for (int i =0; i < 256 ; i++)
		{    
		  unsigned char encours = i;
		  int plusproche = 1 ;
		  for (int j = 2; j <= nbClasses ; j++)
			{              
			  if (EUCLIDEAN(encours, indexClasses[j]) < EUCLIDEAN(encours, indexClasses[plusproche]))
				plusproche = j ;
			}        
		  lut[i] = plusproche ;
		  nbEltsClasses[plusproche] += h[encours] ;
		  sommeClasses[plusproche] += h[encours]*encours ;
		}

	  // On recalcule le centre de chaque classe
	  // a partir des nouveaux germes.
	  for (int j = 1 ; j <= nbClasses ; j++)
		{
		  if (nbEltsClasses[j])
			  sommeClasses[j] /= nbEltsClasses[j] ;
		  else
			  sommeClasses[j] = (unsigned long)mean;
		  if (EUCLIDEAN(indexClasses[j],sommeClasses[j]) > 0.001)  
			  change = true ;
		  indexClasses[j] = (unsigned char)sommeClasses[j] ;
		}
	  nbTours ++ ;
	  std::cout << "Iteration : " << nbTours <<    std::endl ;     
	}

	// on remplace chaque "niveau de Hue" par sa classe
	for (int i=0; i < this->lireNbPixels(); i++) 
		this->operator()(i) = lut[hsv(i)[plan]]; 
}

CImageClasse::CImageClasse(const CImageNdg& im, const std::vector<int>& germes, double tolerance) // croissance région avec ensemble de germes sous la forme x0,y0,x1,y1,x2,y2 etc
{
	this->m_iHauteur = im.lireHauteur();
	this->m_iLargeur = im.lireLargeur();
	this->m_sNom = im.lireNom() + "CR";
	this->m_lNbRegions = germes.size()/2+1;
	this->m_pucPixel = new unsigned long[m_iHauteur*m_iLargeur];
	for (int i = 0; i < this->lireNbPixels(); i++)
		this->operator()(i) = 0;

	// structure PIXEL utilise pour empilements successifs	
	typedef struct {
		int abs;
		int ord;
	} PIXEL;

	for (int cr = 0; cr < (int)germes.size(); cr += 2)
	{
		PIXEL germe;
		germe.abs = germes.at(cr) + 1;
		germe.ord = germes.at(cr+1) + 1;

		CImageClasse growing(im.lireHauteur() + 2, im.lireLargeur() + 2); // suppression des tests pour les bords
		growing.ecrireNbRegions(1);

		CImageNdg img(im.lireHauteur() + 2, im.lireLargeur() + 2, 0);
		for (int i = 0; i < im.lireHauteur(); i++)
			for (int j = 0; j < im.lireLargeur(); j++)
				img(i + 1, j + 1) = im(i, j);

		// gestion des bords -> non nuls pour stopper la croissance
		for (int i = 0; i < growing.lireHauteur(); i++)
		{
			growing(i, 0) = ULONG_MAX;
			growing(i, img.lireLargeur() - 1) = ULONG_MAX;
		}
		for (int j = 0; j < growing.lireLargeur(); j++)
		{
			growing(0, j) = ULONG_MAX;
			growing(img.lireHauteur() - 1, j) = ULONG_MAX;
		}

		boolean encore = true;

		int sInf = int(im(germes.at(cr), germes.at(cr + 1)) - tolerance);
		int sSup = int(im(germes.at(cr), germes.at(cr + 1)) + tolerance);

		std::stack<PIXEL> newMembers, voisins;

		newMembers.push(germe);
		int nbTours = 1;

		while (encore)
		{
			encore = false;
			while (!newMembers.empty()) {
				PIXEL p = newMembers.top();
				newMembers.pop();
				growing(p.abs, p.ord) = growing(p.abs, p.ord) + 1;

				// test des 8 voisins de chaque "germe"
				PIXEL v1;
				v1.abs = p.abs - 1;
				v1.ord = p.ord - 1;

				if ((growing(v1.abs, v1.ord) == 0) && (img(v1.abs, v1.ord) >= sInf) && (img(v1.abs, v1.ord) <= sSup)) {
					growing(v1.abs, v1.ord) = nbTours;
					encore = true;
					voisins.push(v1);
				}

				v1.abs = p.abs - 1;
				v1.ord = p.ord;

				if ((growing(v1.abs, v1.ord) == 0) && (img(v1.abs, v1.ord) >= sInf) && (img(v1.abs, v1.ord) <= sSup)) {
					growing(v1.abs, v1.ord) = nbTours;
					encore = true;
				}

				v1.abs = p.abs - 1;
				v1.ord = p.ord + 1;

				if ((growing(v1.abs, v1.ord) == 0) && (img(v1.abs, v1.ord) >= sInf) && (img(v1.abs, v1.ord) <= sSup)) {
					growing(v1.abs, v1.ord) = nbTours;
					encore = true;
					voisins.push(v1);
				}

				v1.abs = p.abs;
				v1.ord = p.ord - 1;

				if ((growing(v1.abs, v1.ord) == 0) && (img(v1.abs, v1.ord) >= sInf) && (img(v1.abs, v1.ord) <= sSup)) {
					growing(v1.abs, v1.ord) = nbTours;
					encore = true;
					voisins.push(v1);
				}

				v1.abs = p.abs;
				v1.ord = p.ord + 1;

				if ((growing(v1.abs, v1.ord) == 0) && (img(v1.abs, v1.ord) >= sInf) && (img(v1.abs, v1.ord) <= sSup)) {
					growing(v1.abs, v1.ord) = nbTours;
					encore = true;
					voisins.push(v1);
				}

				v1.abs = p.abs + 1;
				v1.ord = p.ord - 1;

				if ((growing(v1.abs, v1.ord) == 0) && (img(v1.abs, v1.ord) >= sInf) && (img(v1.abs, v1.ord) <= sSup)) {
					growing(v1.abs, v1.ord) = nbTours;
					encore = true;
					voisins.push(v1);
				}

				v1.abs = p.abs + 1;
				v1.ord = p.ord;

				if ((growing(v1.abs, v1.ord) == 0) && (img(v1.abs, v1.ord) >= sInf) && (img(v1.abs, v1.ord) <= sSup)) {
					growing(v1.abs, v1.ord) = nbTours;
					encore = true;
					voisins.push(v1);
				}

				v1.abs = p.abs + 1;
				v1.ord = p.ord + 1;

				if ((growing(v1.abs, v1.ord) == 0) && (img(v1.abs, v1.ord) >= sInf) && (img(v1.abs, v1.ord) <= sSup)) {
					growing(v1.abs, v1.ord) = nbTours;
					encore = true;
					voisins.push(v1);
				}
			}
			nbTours += 1;

			if (encore) {
				newMembers.swap(voisins);
			}
		}

		std::cout << "Croissance stable apres " << nbTours << " iterations pour germe " << cr/2 << " ..." << std::endl;

		// on revient à la taille d'origine
		for (int i = 1; i < growing.lireHauteur() - 1; i++)
			for (int j = 1; j < growing.lireLargeur() - 1; j++)
				if ((growing(i, j) >= 1) && (this->operator()(i - 1, j - 1)==0))
						this->operator()(i-1,j-1) = cr/2 + 1;
	}
	// fin de la croissance individuelle de chaque germe image avec 0,1,2 etc
}


CImageClasse::~CImageClasse() {

	if (m_pucPixel) {
		delete[] m_pucPixel;
		m_pucPixel = NULL;
	}
}


void CImageClasse::sauvegarde(const std::string& fixe) {

	CImageNdg temp(this->lireHauteur(),this->lireLargeur());
	temp.choixPalette("binaire");
	temp.ecrireNom(this->lireNom());
	for (int i=0;i<temp.lireNbPixels();i++)
		if (m_pucPixel[i] >= 256) // cast pour rentrer sur un unsigned char
			temp(i) = (unsigned char)(m_pucPixel[i]%255+1);
		else 
			temp(i) = (unsigned char)(m_pucPixel[i]); 
	temp.sauvegarde(fixe);
}

CImageClasse& CImageClasse::operator=(const CImageClasse& im) {

	if (&im == this)
		return *this;

	this->m_iHauteur   = im.lireHauteur();
	this->m_iLargeur   = im.lireLargeur();
	this->m_sNom       = im.lireNom();
	this->m_lNbRegions = im.lireNbRegions();
	this->m_pucPixel   = new unsigned long[m_iHauteur*m_iLargeur];

	if (this->m_pucPixel) 
		delete[] this->m_pucPixel;
	this->m_pucPixel = new unsigned long[im.lireHauteur() * im.lireLargeur()];

	if (im.m_pucPixel != NULL)
		memcpy(this->m_pucPixel,im.m_pucPixel,im.lireNbPixels()*sizeof(unsigned long));

	return *this;
}

// signatures pour Image_Ndg et Image_Couleur
std::vector<SIGNATURE_Ndg> CImageClasse::signatures(const CImageNdg& img, bool enregistrementCSV) {

	std::vector<SIGNATURE_Ndg> tab;

	if (this->lireNbRegions() > 0) {
		tab.resize(this->lireNbRegions()+1); // gestion du fond éventuel, cas des objets, vide si nuées dynamiques
		
		for (int k=0;k<(int)tab.size();k++) {
			tab[k].moyenne = 0;
			tab[k].min = 255;
			tab[k].max = 0;
			tab[k].surface = 0;
		}

		for (int i=0;i<this->lireHauteur();i++)
			for (int j=0;j<this->lireLargeur();j++) {
				tab[this->operator()(i,j)].moyenne += (double)img(i,j);
				if (tab[this->operator()(i, j)].min > img(i, j))
					tab[this->operator()(i, j)].min = img(i, j);
				if (tab[this->operator()(i, j)].max < img(i, j))
					tab[this->operator()(i, j)].max = img(i, j);
				tab[this->operator()(i,j)].surface += 1;
				}

		for (int k=0;k<(int)tab.size();k++) 
			if (tab[k].surface > 0) {
				tab[k].moyenne /= tab[k].surface;
			}

		if (enregistrementCSV) {
		 std::string fichier = "../Res/" + this->lireNom() + "_SNdg.csv";
			std::ofstream f (fichier.c_str());

			if (!f.is_open())
				std::cout << "Impossible d'ouvrir le fichier en ecriture !" << std::endl;
			else {
				f << "Objet; Surface; Min; Max; Moyenne_Ndg " << std::endl;
				for (int k=0;k<(int)tab.size();k++)
					f << k << ";" << tab[k].surface << " ; " << tab[k].min << " ; " << tab[k].max << " ; " << tab[k].moyenne << std::endl;
			}
			f.close();
		}
	}

	return tab;
}

std::vector<SIGNATURE_Couleur> CImageClasse::signatures(const CImageCouleur& img, bool enregistrementCSV) {

	std::vector<SIGNATURE_Couleur> tab;

	if (this->lireNbRegions() > 0) {
		tab.resize(this->lireNbRegions()+1); // gestion de l'"objet" fond

		for (int k=0;k<(int)tab.size();k++) {
			tab[k].moyenne[0] = 0;
			tab[k].moyenne[1] = 0;
			tab[k].moyenne[2] = 0;
			tab[k].surface = 0;
		}

		for (int i=0;i<this->lireHauteur();i++)
			for (int j=0;j<this->lireLargeur();j++) {
				tab[this->operator()(i,j)].moyenne[0] += (double)img(i,j)[0];
				tab[this->operator()(i,j)].moyenne[1] += (double)img(i,j)[1];
				tab[this->operator()(i,j)].moyenne[2] += (double)img(i,j)[2];
				tab[this->operator()(i,j)].surface += 1;
				}

		for (int k=0;k<(int)tab.size();k++) 
			if (tab[k].surface > 0) {
				tab[k].moyenne[0] /= tab[k].surface;
				tab[k].moyenne[1] /= tab[k].surface;
				tab[k].moyenne[2] /= tab[k].surface;
			}

		if (enregistrementCSV) {
		 std::string fichier = "../Res/" + this->lireNom() + "_SCouleur.csv";
			std::ofstream f (fichier.c_str());

			if (!f.is_open())
				std::cout << "Impossible d'ouvrir le fichier en ecriture !" << std::endl;
			else {
				f << "Objet; Surface; Moyenne_0; Moyenne_1; Moyenne_2 " << std::endl;
				for (int k=0;k<(int)tab.size();k++)
					f << k << ";" <<  tab[k].surface << " ; " << tab[k].moyenne[0] << ";" << tab[k].moyenne[1] << " ; " << tab[k].moyenne[2] << std::endl;
			}
			f.close();
		}
	}

	return tab;
}

// affichage
CImageCouleur CImageClasse::affichage(const std::vector<SIGNATURE_Ndg>& tab, int R, int G, int B) {

	CImageCouleur out(this->lireHauteur(),this->lireLargeur());
	out.ecrireNom(this->lireNom() + "_Contours");

		// mettre à jour 

		CImageClasse contours(this->lireHauteur()+2,this->lireLargeur()+2);
		CImageClasse contoursC(this->lireHauteur()+2,this->lireLargeur()+2);
		for (int pix=0;pix<contours.lireNbPixels();pix++)
			contoursC(pix) = contours(pix) = ULONG_MAX;

		for (int i=1;i<contours.lireHauteur()-1;i++)
			for (int j=1;j<contours.lireLargeur()-1;j++) 
				contoursC(i,j) = contours(i,j) = this->operator()(i-1,j-1);

		for (int i=1;i<contours.lireHauteur()-1;i++)
			for (int j=1;j<contours.lireLargeur()-1;j++) {
				int flag = 0;
				int k = i-1;
				while (k <= i+1) {
					int l = j-1;
					while (l <= j+1) {
							if (contours(k,l) != 0)
								flag += 1;
						l++;
					}
					k++;
				}
				if (flag == 9)
					contoursC(i,j) = 0;
			}

		for (int i=1;i<contoursC.lireHauteur()-1;i++) 
			for (int j=1;j<contoursC.lireLargeur()-1;j++)  
				if (contoursC(i,j) != 0) {
					out(i-1,j-1)[0] = R;
					out(i-1,j-1)[1] = G;
					out(i-1,j-1)[2] = B;
				}
				else {
					out(i-1,j-1)[0] = (int)tab[contours(i,j)].moyenne;
					out(i-1,j-1)[1] = (int)tab[contours(i,j)].moyenne;
					out(i-1,j-1)[2] = (int)tab[contours(i,j)].moyenne;
				}

	return out;
}

CImageCouleur CImageClasse::affichage(const std::vector<SIGNATURE_Couleur>& tab, const std::string& methode) {

	CImageCouleur out(this->lireHauteur(),this->lireLargeur());
	out.ecrireNom(this->lireNom());

	if (methode.compare("moyenne") == 0) {
		out.ecrireNom(this->lireNom() + "_Moyenne");

		for (int i=0;i<this->lireNbPixels();i++) {
			out(i)[0] = (int)tab[this->operator()(i)].moyenne[0];
			out(i)[1] = (int)tab[this->operator()(i)].moyenne[1];
			out(i)[2] = (int)tab[this->operator()(i)].moyenne[2];
		}
	}

	return out;
}

// région par région
CImageClasse CImageClasse::selection(const std::string& methode, int classe) {

	CImageClasse out(this->lireHauteur(),this->lireLargeur());
	out.ecrireNom(this->lireNom());
	out.ecrireNbRegions(this->lireNbRegions());

	if (methode.compare("selection") == 0) {
		std::stringstream convert;
		convert << classe;
		out.ecrireNom(this->lireNom() + "_Sel_" + convert.str());

		for (int i=0;i<this->lireNbPixels();i++)
			out(i) = (this->operator()(i) == classe) ? classe : 0;
	}

	return out;
}

// filtrage selon critères taille, bords, etc
CImageClasse CImageClasse::filtrage(const std::string& methode, int tMin, int tMax, bool miseAZero) {

	CImageClasse out(this->lireHauteur(),this->lireLargeur());
	out.ecrireNbRegions(this->lireNbRegions());

	if (methode.compare("taille") == 0) 
	{
		std::stringstream convert;
		convert << tMin << "_" << tMax;
		out.ecrireNom(this->lireNom() + "_Surf_" + convert.str());

		if (miseAZero) {
			std::vector<unsigned long> surface;
			surface.resize(this->lireNbRegions()+1,0);

			for (int i=0;i<this->lireNbPixels();i++)
				surface[this->operator()(i)] += 1; 

			std::vector<unsigned long> lut;
			lut.resize(this->lireNbRegions()+1);
			for (int k =0;k<(int)lut.size();k++)
				lut[k] = ((surface[k] >= (unsigned long)tMin) && (surface[k] <= (unsigned long)tMax)) ? k : 0;

			for (int i=0; i < this->lireNbPixels(); i++) 
				out(i) = lut[this->operator ()(i)]; 
		}
		else {
			std::vector<unsigned long> surface;
			surface.resize(this->lireNbRegions()+1,0);

			for (int i=0;i<this->lireNbPixels();i++)
				surface[this->operator()(i)] += 1; 

			std::vector<unsigned long> lut;
			lut.resize(this->lireNbRegions()+1);
			for (int k = 0;k<(int)lut.size();k++)
				lut[k] = ((surface[k] >= (unsigned long)tMin) && (surface[k] <= (unsigned long)tMax)) ? k : 0;

			CImageClasse out2(this->lireHauteur(),this->lireLargeur());
			for (int i=0; i < this->lireNbPixels(); i++) {
				out(i) = lut[this->operator ()(i)]; 
				out2(i) = lut[this->operator ()(i)]; 
			}

			if (!surface[0]) { // cas ND -> suppression des pixels à 0 par propagation locale, aucunement similarité
				bool onContinue = true;
				int step=1;
				while (onContinue) 	{
					onContinue = false;
					int cpt = 0;
					for (int i=0; i < this->lireHauteur(); i++) 	
						for (int j=0;j < this->lireLargeur();j++) 
							if (out(i,j) == 0) {
								int maxH=0,maxV=0,maxV4=0,maxD1=0,maxD2=0,maxD=0,maxV8=0;

								if (j-1>=0)
									maxH = out(i,j-1);
								if (j+1<(int)this->lireLargeur())
									maxH = max(maxH,(int)out(i,j+1));
								if (i-1>=0)
									maxV = out(i-1,j);
								if (i+1<(int)this->lireHauteur())
									maxV = max(maxV,(int)out(i+1,j));

								maxV4 = max(maxH,maxV);

								if ((i-1>=0) && (j-1>=0))
									maxD1 = out(i-1,j-1);
								if ((i+1<(int)this->lireHauteur()) && (j+1<(int)this->lireLargeur()))
									maxD1 = max(maxD1,(int)out(i+1,j+1));
								if ((i-1>=0) && (j+1<(int)this->lireLargeur()))
									maxD2 = out(i-1,j+1);
								if ((i+1<(int)this->lireHauteur()) && (j-1>=0))
									maxD2 = max(maxD2,(int)out(i+1,j-1));

								maxD = max(maxD1,maxD2);

								maxV8 = max(maxV4,maxD);
								if (maxV8 != 0) {
									out2(i,j) = maxV8;
									onContinue = true;
									cpt++;
								}
							}
					for (int pix=0;pix<this->lireNbPixels();pix++)
							out(pix) = out2(pix);
				}
			}

		// mise à jour des classes

		std::vector<unsigned long> renumerotation;
		renumerotation.resize(out.lireNbRegions()+1,0);

		for (int pix=0;pix<out.lireNbPixels();pix++)
			renumerotation[out(pix)] = out(pix);

		int valEnCours = 1;
		for (int k=0;k<(int)renumerotation.size();k++)
			if (renumerotation[k])
				renumerotation[k] = valEnCours++;
		out.ecrireNbRegions(valEnCours-1);

		for (int i=0; i < out.lireNbPixels(); i++) 
			out(i) = renumerotation[out(i)]; 
		}
	}
	else
		if (methode.compare("maxiLocaux") == 0) 
		{
		int nbBordsi = tMin/2;
		int nbBordsj = tMax / 2;
		out.ecrireNom(this->lireNom() + "ML");
		for (int i=0;i<this->lireHauteur();i++)
			for (int j=0;j<this->lireLargeur();j++) 
				if (this->operator()(i,j) > 0) {
					int dk= max(0,i-nbBordsi);
					int fk= min(i+nbBordsi,this->lireHauteur()-1);
					int dl= max(0,j-nbBordsj);
					int fl= min(j+nbBordsj,this->lireLargeur()-1);
			
					int maxVal = this->operator()(i,j);
					bool flag = true;
					int k = dk;
					while ( (k <= fk) && (flag == true) ) {
						int l=dl;
						while ( (l <= fl) && (flag == true) ) {
								if ((int)this->operator()(k,l) > maxVal)
									flag = false;
							l++;
						}
						k++;
					}
					if (flag == true)
						out(i,j) = this->operator()(i,j);
				}
		}

	return out;
}

// signatures forme pour Image_Ndg et Image_Couleur
std::vector<SIGNATURE_Forme> CImageClasse::signatures(bool enregistrementCSV) {

	std::vector<SIGNATURE_Forme> tab;

	if (this->lireNbRegions() > 0) {
		tab.resize(this->lireNbRegions()+1); // gestion de l'"objet" fond

		for (int k=0;k<(int)tab.size();k++) {
			tab[k].centreGravite_i = 0;
			tab[k].centreGravite_j = 0;
			tab[k].surface = 0;
			tab[k].premierPt_i = -1;
			tab[k].premierPt_j = -1;
			tab[k].codeFreeman = "";
			tab[k].perimetre = 0;
			tab[k].rectEnglob_Bi = 0;
			tab[k].rectEnglob_Bj = 0;
			tab[k].rectEnglob_Hi = this->lireHauteur()-1;
			tab[k].rectEnglob_Hj = this->lireLargeur()-1;
		}

		for (int i=0;i<this->lireHauteur();i++)
			for (int j=0;j<this->lireLargeur();j++) {
				tab[this->operator()(i,j)].centreGravite_i += i;
				tab[this->operator()(i,j)].centreGravite_j += j;
				tab[this->operator()(i,j)].surface += 1;
				if (tab[this->operator()(i,j)].premierPt_i == -1)
					tab[this->operator()(i,j)].premierPt_i = i;
				if (tab[this->operator()(i,j)].premierPt_j == -1)
					tab[this->operator()(i,j)].premierPt_j = j;
				tab[this->operator()(i,j)].rectEnglob_Bi = max(tab[this->operator()(i,j)].rectEnglob_Bi,i);
				tab[this->operator()(i,j)].rectEnglob_Bj = max(tab[this->operator()(i,j)].rectEnglob_Bj,j);
				tab[this->operator()(i,j)].rectEnglob_Hi = min(tab[this->operator()(i,j)].rectEnglob_Hi,i);
				tab[this->operator()(i,j)].rectEnglob_Hj = min(tab[this->operator()(i,j)].rectEnglob_Hj,j);
				}

		for (int k=0;k<(int)tab.size();k++) 
			if (tab[k].surface > 0) {
				tab[k].centreGravite_i /= tab[k].surface;
				tab[k].centreGravite_j /= tab[k].surface;
			}


		CImageClasse agrandie(this->lireHauteur()+2,this->lireLargeur()+2); // bords à 0 pour bonne gestion des contours des objets
		agrandie.ecrireNbRegions(this->lireNbRegions());
		CImageClasse copie(this->lireHauteur(),this->lireLargeur());
		copie.ecrireNbRegions(this->lireNbRegions());

		// presence objets/bord ou ND
		std::vector<unsigned long> surface;
		surface.resize(this->lireNbRegions()+1,0);

		for (int i=0;i<this->lireNbPixels();i++)
			surface[this->operator()(i)] += 1;

		// gestion du coeur
		for (int i=0;i<this->lireHauteur();i++)
			for (int j=0;j<this->lireLargeur();j++) {
				agrandie(i+1,j+1)=this->operator()(i,j);
				copie(i,j)=this->operator()(i,j);
			}

		if (surface[0] > 0) { // cas objets/fond  
			for (int i=1;i<agrandie.lireHauteur()-1;i++)
				for (int j=1;j<agrandie.lireLargeur()-1;j++) 
					if (this->operator()(i-1,j-1) != 0) {
						int minH = min(agrandie(i,j-1),agrandie(i,j+1));
						int minV = min(agrandie(i-1,j),agrandie(i+1,j));
						int minV4 = min(minH,minV);
						copie(i-1,j-1)=minV4;
					}

				for (int pix=0;pix<lireNbPixels();pix++)
					copie(pix) = this->operator()(pix) - copie(pix);
			}
		else { // cas ND
			for (int i=1;i<agrandie.lireHauteur()-1;i++)
				for (int j=1;j<agrandie.lireLargeur()-1;j++) {
						int minH = min(agrandie(i,j-1),agrandie(i,j+1));
						int maxH = max(agrandie(i,j-1),agrandie(i,j+1));
						int minV = min(agrandie(i-1,j),agrandie(i+1,j));
						int maxV = max(agrandie(i-1,j),agrandie(i+1,j));
						int minV4 = min(minH,minV);
						int maxV4 = max(maxH,maxV);
						if (!( (agrandie(i,j) == minV4) && (agrandie(i,j) == maxV4) )) // pixel différent de ses voisins
							copie(i-1,j-1)=0;
					}

				for (int pix=0;pix<lireNbPixels();pix++)
					copie(pix) = this->operator()(pix) - copie(pix);
		}
		// 	copie -> image des bords intérieurs des objets
		
		// parcours dans sens jusqu'à rejoindre point initial ou "stabilisation" au même point

		for (int num = 1; num <= this->lireNbRegions();num++) {
			int pIniti = tab[num].premierPt_i;
			int pInitj = tab[num].premierPt_j;

			int pi = pIniti;
			int pj = pInitj;

			bool retour = false;

			while (!retour) {
								
				int pi_encours = pi;
				int pj_encours = pj;

				if ((pj != copie.lireLargeur()-1) && (copie(pi,pj+1) == num)) {
					tab[num].codeFreeman += "0";
					copie(pi,pj+1) = 0;
					pj = pj+1;
				}
				else
					if ( (pi != copie.lireHauteur()-1) && (pj != copie.lireLargeur()-1) && (copie(pi+1,pj+1) == num) ) {
					tab[num].codeFreeman += "7";
					copie(pi+1,pj+1) = 0;
					pi = pi+1;
					pj = pj+1;
					}
					else
						if ( (pi != copie.lireHauteur()-1) && (copie(pi+1,pj) == num)) {
						tab[num].codeFreeman += "6";
						copie(pi+1,pj)=0;
						pi = pi+1;
						}
						else
							if ( (pi != copie.lireHauteur()-1) && (pj != 0) && (copie(pi+1,pj-1) == num)) {
							tab[num].codeFreeman += "5";
							copie(pi+1,pj-1)=0;
							pi = pi+1;
							pj = pj-1;
							}
							else
								if ( (pj != 0) && (copie(pi,pj-1) == num)) {
								tab[num].codeFreeman += "4";
								copie(pi,pj-1)=0;
								pj = pj-1;
								}
								else
									if ( (pi != 0) && (pj != 0) && (copie(pi-1,pj-1) == num)) {
									tab[num].codeFreeman += "3";
									copie(pi-1,pj-1)=0;
									pi = pi-1;
									pj = pj-1;
									}
									else
										if ( (pi != 0) && (copie(pi-1,pj) == num)) {
										tab[num].codeFreeman += "2";
										copie(pi-1,pj)=0;
										pi = pi-1;
										}
										else
											if ( (pi != 0) && (pj != copie.lireLargeur()-1) && (copie(pi-1,pj+1) == num)) {
											tab[num].codeFreeman += "1";
											copie(pi-1,pj+1)=0;
											pi = pi-1;
											pj = pj+1;
											}

				if ( (((pi == pIniti) && (pj == pInitj))) || ((pi == pi_encours) && (pj == pj_encours)) )
					retour = true;
			}
		}

		// calcul du périmètre

		for (int num = 1; num <= this->lireNbRegions();num++) {
			for (int code=0;code<(int)tab[num].codeFreeman.size();code++) {
				if ((tab[num].codeFreeman[code] == '0') || (tab[num].codeFreeman[code] == '2') || (tab[num].codeFreeman[code] == '4') || (tab[num].codeFreeman[code] == '6'))
					tab[num].perimetre += 1;
				else
					tab[num].perimetre += (float)sqrt(2);	
			}
		}

		if (enregistrementCSV) {
		 std::string fichier = "../Res/" + this->lireNom() + "_SForme.csv";
			std::ofstream f (fichier.c_str());

			if (!f.is_open())
				std::cout << "Impossible d'ouvrir le fichier en ecriture !" << std::endl;
			else {
				f << "Objet; CG_i; CG_j; Surface; codeFreeman ; RE_Hi ; RE_Hj ; RE_Bi ; RE_Bj ; perimetre" << std::endl;
				for (int k=0;k<(int)tab.size();k++)
					f << k << ";" << tab[k].centreGravite_i << ";" << tab[k].centreGravite_j << ";" << tab[k].surface << ";" << tab[k].codeFreeman << " ; " << tab[k].rectEnglob_Hi << " ; " << tab[k].rectEnglob_Hj << " ; " << tab[k].rectEnglob_Bi << " ; " << tab[k].rectEnglob_Bj << " ; " << tab[k].perimetre << std::endl;
			}
			f.close();
		}
	}

	return tab;
}

CImageClasse CImageClasse::mutation(const CImageNdg& img)
{
	
	CImageClasse out(img.lireHauteur(),img.lireLargeur());
	out.ecrireNom(img.lireNom()+"M");
	out.ecrireNbRegions(1);

	for (int i=0;i<img.lireNbPixels();i++)
		out(i) = (img(i) > 0) ? 1UL : 0UL;

	return out;
}

CImageNdg CImageClasse::mutation(const std::string& methode) // ou "troncature" pour truncature au-dessus de 255
{															
															
	CImageNdg out(this->lireHauteur(),this->lireLargeur());
	out.ecrireNom(this->lireNom()+"T");
	out.choixPalette("binaire");

	if (methode.compare("troncature") == 0 ) {
		for (int i=0;i<this->lireNbPixels();i++)
			if (this->operator()(i) == 0UL)
				out(i) = 0;
			else
				if (this->operator()(i) < 256)
					out(i) = (unsigned char)this->operator()(i);
				else
					out(i) = (unsigned char)255;
	}

	return out;
}

// cellules de Voronoï
CImageClasse CImageClasse::cellules()
{

	CImageClasse out(this->lireHauteur(), this->lireLargeur());
	out.ecrireNom(this->lireNom() + "CV");
	out.ecrireNbRegions(this->lireNbRegions());

	// structure PIXEL utilise pour empilements successifs	
	typedef struct {
		int abs;
		int ord;
		long value;
	} PIXEL;
	std::stack<PIXEL> newMembers, voisins;

	CImageClasse growing(this->lireHauteur() + 2, this->lireLargeur() + 2); // suppression des tests pour les bords

	for (int i = 0; i < this->lireHauteur(); i++)
		for (int j = 0; j < this->lireLargeur(); j++)
			growing(i + 1, j + 1) = this->operator()(i, j);

	// gestion des bords -> non nuls pour stopper la croissance
	for (int i = 0; i < growing.lireHauteur(); i++)
	{
		growing(i, 0) = ULONG_MAX;
		growing(i, growing.lireLargeur() - 1) = ULONG_MAX;
	}
	for (int j = 0; j < growing.lireLargeur(); j++)
	{
		growing(0, j) = ULONG_MAX;
		growing(growing.lireHauteur() - 1, j) = ULONG_MAX;
	}

	// initialisation des germes
	for (int i = 1; i < this->lireHauteur()-1; i++)
		for (int j = 1; j < this->lireLargeur()-1; j++)
			if (growing(i, j) != 0)
			{
				PIXEL pxl;
				pxl.abs = i;
				pxl.ord = j;
				pxl.value = growing(i, j);

				newMembers.push(pxl);
			}

	bool encore = true;
	while (encore)
		{
			encore = false;
			while (!newMembers.empty()) {
				PIXEL p = newMembers.top();
				
				newMembers.pop();

				// test des 8 voisins de chaque "germe"
				PIXEL v1;
				v1.abs = p.abs - 1;
				v1.ord = p.ord - 1;
				v1.value = p.value;

				if (growing(v1.abs, v1.ord) == 0) {
					growing(v1.abs, v1.ord) = v1.value;
					encore = true;
					voisins.push(v1);
				}

				v1.abs = p.abs - 1;
				v1.ord = p.ord;
				v1.value = p.value;

				if (growing(v1.abs, v1.ord) == 0) {
					growing(v1.abs, v1.ord) = v1.value;
					encore = true;
				}

				v1.abs = p.abs - 1;
				v1.ord = p.ord + 1;
				v1.value = p.value;

				if (growing(v1.abs, v1.ord) == 0) {
					growing(v1.abs, v1.ord) = v1.value;
					encore = true;
					voisins.push(v1);
				}

				v1.abs = p.abs;
				v1.ord = p.ord - 1;
				v1.value = p.value;

				if (growing(v1.abs, v1.ord) == 0) {
					growing(v1.abs, v1.ord) = v1.value;
					encore = true;
					voisins.push(v1);
				}

				v1.abs = p.abs;
				v1.ord = p.ord + 1;
				v1.value = p.value;

				if (growing(v1.abs, v1.ord) == 0) {
					growing(v1.abs, v1.ord) = v1.value;
					encore = true;
					voisins.push(v1);
				}

				v1.abs = p.abs + 1;
				v1.ord = p.ord - 1;
				v1.value = p.value;

				if (growing(v1.abs, v1.ord) == 0) {
					growing(v1.abs, v1.ord) = v1.value;
					encore = true;
					voisins.push(v1);
				}

				v1.abs = p.abs + 1;
				v1.ord = p.ord;
				v1.value = p.value;

				if (growing(v1.abs, v1.ord) == 0) {
					growing(v1.abs, v1.ord) = v1.value;
					encore = true;
					voisins.push(v1);
				}

				v1.abs = p.abs + 1;
				v1.ord = p.ord + 1;
				v1.value = p.value;

				if (growing(v1.abs, v1.ord) == 0) {
					growing(v1.abs, v1.ord) = v1.value;
					encore = true;
					voisins.push(v1);
				}
			}

			if (encore) {
				newMembers.swap(voisins);
			}
		}

		// on revient à la taille d'origine
		for (int i = 1; i < growing.lireHauteur() - 1; i++)
			for (int j = 1; j < growing.lireLargeur() - 1; j++)
				out(i - 1, j - 1) = growing(i,j);

		return out;
}

std::vector<SIGNATURE_Cellule> CImageClasse::sigCellules(const CImageNdg& img, bool enregistrementCSV)
{
	CImageClasse CV = this->cellules();

	std::vector<SIGNATURE_Cellule> tab;


	if (this->lireNbRegions() > 0) {
		tab.resize(this->lireNbRegions() + 1); // gestion de l'"objet" fond
		std::vector<double> ngObj;
		ngObj.resize(this->lireNbRegions() + 1,0);

		for (int k = 0; k<(int)tab.size(); k++) {
			tab[k].surface = 0;
			tab[k].emergence = 0;
			tab[k].occupation = 0;
		}

		for (int i = 0; i < this->lireNbPixels(); i++) {
				tab[CV(i)].surface += 1;
				tab[CV(i)].emergence += img(i)*((CV(i) - this->operator()(i))/CV(i));
				tab[this->operator()(i)].occupation += 1;
				ngObj[this->operator()(i)] += img(i);
			}

		for (int k = 0; k < (int)tab.size(); k++)
		{
			tab[k].emergence = (ngObj[k] / tab[k].occupation) - (tab[k].emergence / (tab[k].surface - tab[k].occupation));
			if (tab[k].surface > 0) // fond
				tab[k].occupation = tab[k].occupation / tab[k].surface;
			else
				tab[k].occupation = 0;
		}

		if (enregistrementCSV) {
			std::string fichier = "../Res/" + this->lireNom() + "_SVoronoi.csv";
			std::ofstream f(fichier.c_str());

			if (!f.is_open())
				std::cout << "Impossible d'ouvrir le fichier en ecriture !" << std::endl;
			else {
				f << "Objet; Surface; emergence ; occupation" << std::endl;
				for (int k = 0; k<(int)tab.size(); k++)
					f << k << ";" << tab[k].surface << ";" << tab[k].emergence << ";" << tab[k].occupation << std::endl;
			}
			f.close();
		}
	}

	return tab;
}
