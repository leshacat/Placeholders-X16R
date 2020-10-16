// Copyright (c) 2011-2016 The Bitcoin Core developers
// Copyright (c) 2017 The Placeholder Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "deployvmdialog.h"
#include "sendcoinsdialog.h"
#include "ui_deployvmdialog.h"

#include "addresstablemodel.h"
#include "placehunits.h"
#include "clientmodel.h"
#include "assetcontroldialog.h"
#include "guiutil.h"
#include "optionsmodel.h"
#include "platformstyle.h"
#include "sendassetsentry.h"
#include "walletmodel.h"

#include "base58.h"
#include "chainparams.h"
#include "wallet/coincontrol.h"
#include "validation.h" // mempool and minRelayTxFee
#include "ui_interface.h"
#include "txmempool.h"
#include "policy/fees.h"
#include "wallet/fees.h"
#include "createassetdialog.h"
#include "reissueassetdialog.h"
#include "crc32.h"
#include "placeholderutility.h"

#include <iostream>
#include <fstream>
#include <string>
#include <thread>

#include <QtWidgets>

#include <QString>
#include <QDir>
#include <QProcess>
#include <QFontMetrics>
#include <QMessageBox>
#include <QScrollBar>
#include <QSettings>
#include <QLabel>
	
#include <QHBoxLayout>
#include <QTextDocument>
#include <QTableWidget>
#include <QTimer>
#include <policy/policy.h>
#include <core_io.h>
#include <rpc/mining.h>

#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkAccessManager>
#include <QHttpMultiPart>
#include <QByteArray>
#include <QDesktopServices>

#include <QDialog>
#include <QMessageBox>
#include <QString>
#include <QTimer>
#include <QTextEdit>
#include <QGroupBox>
#include <QCheckBox>
#include <QLabel>
#include <QFileDialog>

#include <QLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QSpinBox>
#include <QWidget>
#include <QComboBox>
#include <QVBoxLayout>
#include <QPalette>


#include <zlib.h>


DeployVMDialog::DeployVMDialog(const PlatformStyle *_platformStyle, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::DeployVMDialog),
        clientModel(0),
        model(0)     
{

	try { 
		platformStyle = _platformStyle;
		bigEditor = new QTextEdit;
		bigEditor->setPlainText(tr("Ready..."));

	 	
		createHorizontalGroupBox();

		
		buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

		connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
		connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
		
		QVBoxLayout *mainLayout = new QVBoxLayout;
		 
		mainLayout->addWidget(horizontalGroupBox);

		mainLayout->addWidget(bigEditor);
		//mainLayout->addWidget(buttonBox);
		
		setLayout(mainLayout);

		setWindowTitle(tr("Deploy To Network"));
	
	} catch(...) { } 
	
}

void DeployVMDialog::createHorizontalGroupBox()
{
	try { 
	int maxHeight = 100;
	int margin    = 15;
	
	QPalette *palette = new QPalette();
	palette->setColor(QPalette::Base,Qt::lightGray);
	palette->setColor(QPalette::Text,Qt::darkGray);
	
	
    horizontalGroupBox = new QGroupBox(tr("Network Deployment Details"));
	horizontalGroupBox->setFixedHeight(maxHeight);
	
	QVBoxLayout *base = new QVBoxLayout;
	
    QHBoxLayout *layoutTop = new QHBoxLayout;
    QHBoxLayout *layoutBottom = new QHBoxLayout;

    QPushButton* browseButton = new QPushButton(tr("Browse for File"));
		
    layoutTop->addWidget(browseButton);
	
	QLabel* fileInformationLabel = new QLabel(tr("File Path"));		
	fileInformationLabel->setToolTip("This is the path of the file");
	
	//fileInformation->setMaximumWidth(100);
	//fileInformation->setFixedWidth(80);
	fileInformation->setReadOnly(true);
	fileInformation->setPalette(*palette);
	fileInformation->setPlaceholderText("File Path");
	fileInformation->setToolTip("The file and file path selected for deployment.");
	
	layoutTop->addWidget(fileInformationLabel);
	layoutTop->addWidget(fileInformation);		
	
	QLabel* checksumLabel = new QLabel(tr("Checksum"));
	checksumLabel->setToolTip("A digit representing the sum of the correct digits in a piece of stored or transmitted digital data, against which later comparisons can be made to detect errors in the data.");
	
    layoutTop->addWidget(checksumLabel);
		
	checksum->setMaximumWidth(100);
	checksum->setFixedWidth(80);
	checksum->setReadOnly(true);
	checksum->setPlaceholderText("CRC32B");
	checksum->setAlignment(Qt::AlignRight);
	checksum->setToolTip("A digit representing the sum of the correct digits in a piece of stored or transmitted digital data, against which later comparisons can be made to detect errors in the data.");
	

	checksum->setPalette(*palette);

	layoutTop->addWidget(checksum);
		
	QLabel* artifactLabel = new QLabel(tr("Artifact"));		
	artifactLabel->setToolTip("This is the address of the artifact itself and also the payment address");
	layoutTop->addWidget(artifactLabel);
	artifact->setPalette(*palette);
	artifact->setPlaceholderText("Eg. F9ddp3zXbNd5zwxBngvRBe4zQgmoViNuma");
	artifact->setToolTip("This is the address of the artifact itself and also the payment address");
	artifact->setReadOnly(true);
	
	layoutTop->addWidget(artifact);
	QPushButton* generateNewAddressButton = new QPushButton(tr(""));
	generateNewAddressButton->setIcon(platformStyle->SingleColorIcon(":/icons/receiving_addresses"));
	
	
	
	signature->setToolTip("The signature includes the checksum of the file, the artifact address and a date stamp to validate the signer, contents, and time it was deployed.");
	signature->setPlaceholderText("Generated by clicking Sign");
	signature->setPalette(*palette);
	signature->setReadOnly(true);
	
	signButton = new QPushButton(tr("Sign"));
	signButton->setToolTip("Sign this artifact for deployment");
	QLabel* signatureLabel = new QLabel(tr("Signature"));		
	layoutTop->addWidget(signatureLabel);
	layoutTop->addWidget(signature);
	//layoutTop->addWidget(signButton);
		
	QLabel* descriptionLabel = new QLabel(tr("Description"));	
	layoutBottom->addWidget(descriptionLabel);
	description->setToolTip("The description will help users determine the contents of the artifact.");
	description->setPlaceholderText("Add a useful description for your artifact");
	layoutBottom->addWidget(description);

	QLabel* tagsLabel = new QLabel(tr("Tags"));	

	layoutBottom->addWidget(tagsLabel);
	tags->setToolTip("The tags will help users sort the repository based on user input. Seperate by comma.");
	tags->setPlaceholderText("Add useful tags/keywords to describe your comments. ");
	tags->setMaximumWidth(80);
	tags->setFixedWidth(200);
	layoutBottom->addWidget(tags);

	
	QLabel* bountyLabel = new QLabel(tr("Purchase Price (PHL)"));
	layoutBottom->addWidget(bountyLabel);

	bounty->setMaximumWidth(80);
	bounty->setFixedWidth(100);
	bounty->setToolTip("The price you wish to receive for each download of this content");
	bounty->setAlignment(Qt::AlignRight);
	bounty->setPlaceholderText("0.00000000");
		
	layoutBottom->addWidget(bounty);
		
	deployToNetworkButton = new QPushButton(tr("Deploy to Network"));

	deployToNetworkButton->setEnabled(false);
		
	viewOnExplorerButton = new QPushButton(tr("View on Explorer"));

    layoutTop->addWidget(deployToNetworkButton);
    layoutTop->addWidget(viewOnExplorerButton);
		
	clearConsoleButton = new QPushButton(tr("Clear"));
	layoutBottom->addWidget(clearConsoleButton);		

	connect(viewOnExplorerButton, SIGNAL(clicked()), this, SLOT(viewOnExplorer()));
	connect(generateNewAddressButton, SIGNAL(clicked()), this, SLOT(generateNewAddress()));
	connect(clearConsoleButton, SIGNAL(clicked()), this, SLOT(clearAll()));
	connect(browseButton, SIGNAL(clicked()), this, SLOT(selectFile()));
	connect(signButton, SIGNAL(clicked()), this, SLOT(signMessage()));
	connect(deployToNetworkButton, SIGNAL(clicked()), this, SLOT(deployToNetwork()));
		
	base->addLayout(layoutTop);
	base->addLayout(layoutBottom);
	//
	horizontalGroupBox->setLayout(base);
	
	} catch(...) { } 
}
/*

MOVE MAIN CODE IN THIS FUNCTION TO PLACEHOLDER UTILITY 

*/

void DeployVMDialog::clearAll()
{
	try {
		fileName = "";
		bigEditor->clear();
		bounty->setText("");
		artifact->setText("");
		fileInformation->setText("");
		signature->setText("");
		checksum->setText("");
		tags->setText("");
		
		description->setText("");
		deployToNetworkButton->setEnabled(false);
	} catch(...) { }
	
}

void DeployVMDialog::generateNewAddress()
{
	try { 
	CWallet *pwallet = model->getWallet();
    CPubKey newKey;
    if (!pwallet->GetKeyFromPool(newKey)) {
        //throw JSONRPCError(RPC_WALLET_KEYPOOL_RAN_OUT, "Error: Keypool ran out, please call keypoolrefill first");
		
	}
    //CKeyID keyID = newKey.GetID();
	
	CKeyID id = newKey.GetID();
	CPlacehAddress address(id);
	if (address.IsValid()) {
		//cout << "valid" << endl;    
	} else {
		//cout << "invalid" << endl;
	}

	//cout << address.ToString() << endl; /

	artifact->setText(QString::fromStdString(address.ToString()));
	} catch(...) {}  
}

void DeployVMDialog::selectFile()
{
	try { 
		fileName = QFileDialog::getOpenFileName(this, ("Open File"),
													  "/home",
													  ("Artifact (*.png *.gif *.jpg *.gpg *.pgp *.torrent *.txt *.zip *.html *.xml *.htm *.mov *.mpeg *.mp3 *.js *.pdf *.vdi *.json *.jar *class)"));
													  
		QFile *file = new QFile(fileName);
		if( file->size() > 2000000 ) { 
			QMessageBox msgBoxError;
			msgBoxError.setText("This file is too large. Please convert it into a torrent first.");
			msgBoxError.exec();														
			return;
		}
		
		fileInformation->setText(fileName);

		deployToNetworkButton->setEnabled(true);
		Crc32 *crc32 = new Crc32();
		
		quint32 crc = crc32->calculateFromFile(fileName);
			
		QString cs = QString::number(crc);
		checksum->setText(cs);
		
	
		PlaceholderUtility* pu = new PlaceholderUtility();
		
		contentMimeType = pu->getContentTypeByExtension(fileName);
		
		generateNewAddress();
		
		signMessage();
		
	}
	catch(...) { 
		QMessageBox msgBoxC;
		msgBoxC.setText("An error occurred mapping the MIME type");
		msgBoxC.exec();				
	}
}


void DeployVMDialog::viewOnExplorer()
{
	QString address = artifact->text();
	if (address.trimmed().isEmpty()){
		QMessageBox msgBoxC;
		msgBoxC.setText("No Artifact!!!");
		msgBoxC.exec();	
		
		console("No Artifact!!!");
		console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
		
		return;
	}
	console("Open browser");
	console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
	
	//QString address = artifact->text();
	PlaceholderUtility* pu = new PlaceholderUtility();
	QString link = pu->getArtifactJson() + address.trimmed() + ".json";
	QDesktopServices::openUrl(QUrl(link));
	console("OK");
}


void DeployVMDialog::console(const char* message)
{
	bigEditor->setText(bigEditor->toPlainText() + "" + message + "\n");		
		
	QApplication::processEvents();
		
	QTextCursor cursor1= bigEditor->textCursor();
	cursor1.atEnd();
	bigEditor->setTextCursor(cursor1);
	bigEditor->ensureCursorVisible();
	bigEditor->verticalScrollBar()->setValue(bigEditor->verticalScrollBar()->maximum());
	bigEditor->repaint();
}

void DeployVMDialog::deployToNetwork()
{
	if( signature->text().trimmed().isEmpty() ) { 
		console("WARNING: BAD SIGNATURE!!!");
		console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");

		QMessageBox msgBoxC;
		msgBoxC.setText("Bad signature!");
		msgBoxC.exec();	
		return;
	}
	
	if( checksum->text().trimmed().isEmpty() ) { 
		console("WARNING: NO CHECKSUM!!!");
		console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
		QMessageBox msgBoxC;
		msgBoxC.setText("No checksum!");
		msgBoxC.exec();	
		return;
	}
	
	if( checksum->text().trimmed().toUInt() == 0 ) { 
		QMessageBox msgBoxC;
		msgBoxC.setText("Invalid Checksum, select a file to deploy");
		msgBoxC.exec();	
		return;
	}
	
	
	if( tags->text().trimmed().isEmpty() ) { 
		console("WARNING: NO TAGS!!!");
		console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
		QMessageBox msgBoxC;
		msgBoxC.setText("Please enter a few helpful tags/keywords seperated by commas.");
		msgBoxC.exec();	
		return;
	}
	
	
	if( artifact->text().trimmed().isEmpty()) { 
		QMessageBox msgBoxC;
		msgBoxC.setText("No Artifact!");
		msgBoxC.exec();	
		artifact->setFocus();
		return;
	}
	
	if( !model->validateAddress(artifact->text().trimmed())) {
		QMessageBox msgBoxC;
		msgBoxC.setText("Invalid address/artifact!");
		msgBoxC.exec();	
		artifact->setFocus();
		return;
	}
	
	if( description->text().trimmed().isEmpty()) { 
		QMessageBox msgBoxC;
		msgBoxC.setText("No Description!");
		msgBoxC.exec();	
		description->setFocus();
		return;
	}
	
	console("Create Network Access Manager");
	console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
	QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);
	console("Connecting signals and slots to network reply...");
	console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
	connect(networkManager, SIGNAL(finished(QNetworkReply*)),
			SLOT(onPostAnswer(QNetworkReply*)));

	console("Locating Network Repository");
	console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
	
	PlaceholderUtility* pu = new PlaceholderUtility();
	QUrl testUrl(pu->getDeployEntryPointURL());
	QUrlQuery query;
	//QUrl params;
	QByteArray postData;

	console("Setting MIME type");
	console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
	
	query.addQueryItem("contentType", contentMimeType);//"application/x-bittorrent");
	query.addQueryItem("verifySignature", "-");
	
	console("Using Signature");
	console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
	
	query.addQueryItem("signature", signature->text());
	
	
	query.addQueryItem("crc32Long", checksum->text());
	query.addQueryItem("crc32Hex", "-");
	query.addQueryItem("checksum", checksum->text());
	
	query.addQueryItem("verifyCommand", "-");
	//query.addQueryItem("file", "-=-=-=-=-=-=-");
	console("Loading to artifact");
	console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
	//console("Using Artifact:" + artifact->text());
	
	query.addQueryItem("artifact", artifact->text());
	query.addQueryItem("address", artifact->text());
	query.addQueryItem("bounty", bounty->text());
	query.addQueryItem("tags", tags->text());
	query.addQueryItem("description", description->text());
	query.addQueryItem("data", "-=" + artifact->text() + "=- Created with 2.0.30.4d");
	console("Setting network parameters...");
	console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");

	postData = testUrl.toEncoded(QUrl::RemoveFragment);
	
	testUrl.setQuery(query);
	console("Encoding fragment...");
	console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
	
	QNetworkRequest request;
	request.setUrl(testUrl);
	console("Setting Raw Header");
	console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
	request.setRawHeader("User-Agent", "Placeholders-X16R 2.0.30.1");

	console("Creating Multipart HTTP Form Data");
	console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");

	QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

	QString preview_path  = fileName; 
	QHttpPart previewPathPart;
	previewPathPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"path\""));
	previewPathPart.setBody(preview_path.toLatin1());
	console("Body Loaded");
	console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");

	QHttpPart previewFilePart;
	previewFilePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant( "image/jpeg"));
	previewFilePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file\""));
	QFile *file = new QFile(preview_path);
	
	console("File check");
	console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
	if (!file->exists()) {
		console("Upload Error. File does not exist. Aborting! ");
		console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
		//emit error(tr("Upload Error. File does not exist: ") + preview_path);
		return;
	}	
	console("Open I/O");
	console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
	file->open(QIODevice::ReadOnly);
	previewFilePart.setBodyDevice(file);
	file->setParent(multiPart); // we cannot delete the file now, so delete it with the multiPart
	console("Appending data");
	console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
	
	multiPart->append(previewPathPart);
	multiPart->append(previewFilePart);
	console("Post Data to Network");
	console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
	
	QNetworkReply* reply = networkManager->post(request, multiPart);
	//multiPart->setParent(reply); // delete the multiPart with the reply
	
	console("Connecting SIGNAL");
	console(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");

	connect(networkManager, SIGNAL(finished(QNetworkReply*)),
			SLOT(onPostAnswer(QNetworkReply*)));
	
	console("Complete");


	QMessageBox msgBoxC;
	msgBoxC.setText("Your file has been successfully added to marketplace.");
	msgBoxC.exec();	
		
}

void DeployVMDialog::signMessage()
{
    if (!model)
        return;

    /* Clear old signature to ensure users don't get confused on error with an old signature displayed */
    signature->setText("");

    CTxDestination destination = DecodeDestination(artifact->text().toStdString());
    if (!IsValidDestination(destination)) {
		console("The entered address is invalid");
        //ui->statusLabel_SM->setStyleSheet("QLabel { color: red; }");
        //ui->statusLabel_SM->setText(tr("The entered address is invalid.") + QString(" ") + tr("Please check the address and try again."));
        return;
    }
    const CKeyID* keyID = boost::get<CKeyID>(&destination);
    if (!keyID) {
		console("The entered address does not refer to a key");
        //ui->addressIn_SM->setValid(false);
        //ui->statusLabel_SM->setStyleSheet("QLabel { color: red; }");
        //ui->statusLabel_SM->setText(tr("The entered address does not refer to a key.") + QString(" ") + tr("Please check the address and try again."));
        return;
    }

    WalletModel::UnlockContext ctx(model->requestUnlock());
    if (!ctx.isValid())
    {
        console("The wallet unlock was cancelled");
        return;
    }

    CKey key;
    if (!model->getPrivKey(*keyID, key))
    {
        console("Private key for the entered address is not available.");
        return;
    }

    CHashWriter ss(SER_GETHASH, 0);
    ss << strMessageMagic;
    ss << artifact->text().toStdString() + " " + checksum->text().toStdString();

    std::vector<unsigned char> vchSig;
    if (!key.SignCompact(ss.GetHash(), vchSig))
    {
        console("Message signing failed.");
        return;
    }

    console("Message signed.");

    signature->setText(QString::fromStdString(EncodeBase64(vchSig.data(), vchSig.size())));
}

void DeployVMDialog::setClientModel(ClientModel *_clientModel)
{
    this->clientModel = _clientModel;
}



void DeployVMDialog::setModel(WalletModel *_model)
{
    this->model = _model;
}

DeployVMDialog::~DeployVMDialog()
{
   	try { 
		delete ui;
	} catch(...) { } 
}

/** PHL END */
