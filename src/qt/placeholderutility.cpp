
#include "placeholderutility.h"

void PlaceholderUtility::updateList() 
{
		try { 
			std::remove(repositoryListFile.toUtf8().constData());
	
			QProcess grabListProcess;
			QString grabList =  aria2cPath + pathSeperator + aria2cName + " --allow-overwrite --conditional-get=true --out=list.json --dir=" + repositoryPath + " " + seedListURL;
			grabListProcess.start(grabList);
			grabListProcess.waitForFinished();
			grabListProcess.close();

		} catch(...) { 
			QMessageBox msgBoxError;
			msgBoxError.setText("An error was encountered trying to update the Placeholder Artifact list for synchronization");
			msgBoxError.exec();
		}
	
}

void PlaceholderUtility::updateList(QString criteria) 
{
		try { 
			std::remove(repositoryListFile.toUtf8().constData());
	
			QProcess grabListProcess;
			QString grabList =  aria2cPath + pathSeperator + aria2cName + " --allow-overwrite --conditional-get=true --out=list.json --dir=" + repositoryPath + " " + seedListURL + "tags:" + criteria;
			grabListProcess.start(grabList);
			grabListProcess.waitForFinished();
			grabListProcess.close();

		} catch(...) { 
			QMessageBox msgBoxError;
			msgBoxError.setText("An error was encountered trying to update the Placeholder Artifact list for synchronization");
			msgBoxError.exec();
		}	
}

void PlaceholderUtility::updateRecentList() 
{
		try { 
			std::remove(repositoryListFile.toUtf8().constData());
	
			QProcess grabListProcess;
			QString grabList =  aria2cPath + pathSeperator + aria2cName + " --allow-overwrite --conditional-get=true --out=list.json --dir=" + repositoryPath + " " + seedListURL + "tags:recent";
			grabListProcess.start(grabList);
			grabListProcess.waitForFinished();
			grabListProcess.close();

		} catch(...) { 
			QMessageBox msgBoxError;
			msgBoxError.setText("An error was encountered trying to update the Placeholder Artifact list for synchronization");
			msgBoxError.exec();
		}	
}

void PlaceholderUtility::consume(QString artifact)
{

		try { 
		
			// do checksum
			// check if .artifact exists
		
			QProcess task;
			QString executionPath =  launchCmd + javaName + " -jar " + repositoryPath + pathSeperator + artifact + ".jar";
			task.start(executionPath);
			task.waitForFinished();
			task.close();

		} catch(...) { 
			QMessageBox msgBoxError;
			msgBoxError.setText("An error was encountered trying to process Placeholder Artifact task");
			msgBoxError.exec();
		}	
}

PlaceholderUtility::PlaceholderUtility() 
{ 
	
	try { 
		QString settingsFile = QApplication::applicationDirPath() + "/settings.ini";
		QSettings settings(settingsFile, QSettings::IniFormat);
		vdiPath = settings.value("vdiPath", "").toString();
		virtualBoxManage = settings.value("virtualBoxManage", "").toString();
		placeholderPath = settings.value("placeholderPath", "").toString();
		aria2cPath = settings.value("aria2cPath", "").toString();
		repositoryPath = settings.value("repositoryPath", "").toString();
		repositoryListFile = settings.value("repositoryListFile", "").toString();
		seedListURL = settings.value("seedListURL", "").toString();
		artifactSeedAnnounceURL = settings.value("artifactSeedAnnounceURL", "").toString();
		artifactDetailURL = settings.value("artifactDetailURL", "").toString();
		artifactJson = settings.value("artifactJson", "").toString();
		registerServiceEndPointURL = settings.value("registerServiceEndPointURL", "").toString();
		paymentAddress = settings.value("paymentAddress", "").toString();
		aria2cName = settings.value("aria2cName", "").toString();
		javaName = settings.value("javaName", "").toString();
		pathSeperator = settings.value("pathSeperator", "").toString();
		launchCmd = settings.value("launchCmd","").toString();
		earn = settings.value("earn","").toString();
		
		
		userid = settings.value("userid", "").toString();
		password = settings.value("password", "").toString();

	} catch(...) { 
		QMessageBox msgBoxError;
		msgBoxError.setText("An error was encountered trying to access the Placeholder settings.ini file");
		msgBoxError.exec();
	}
	
}

QString PlaceholderUtility::getMacAddress()
{
	QList<QNetworkInterface> list =	QNetworkInterface::allInterfaces();
	for(int i = 0; i < list.count(); i++ ) { 
		
		QNetworkInterface netInterface = list.at(i);
        // Return only the first non-loopback MAC Address
        if (!(netInterface.flags() & QNetworkInterface::IsLoopBack))
            return netInterface.hardwareAddress();
    }
    return QString();
}


QString PlaceholderUtility::getRepositoryListFile()
{
	return repositoryListFile;
}

QString PlaceholderUtility::getEarn()
{
	return earn;
}

QString PlaceholderUtility::getUserID()
{
	return userid;
}

QString PlaceholderUtility::getPassword()
{
	return password;
}

QString PlaceholderUtility::getPlaceholderPath()
{
	return placeholderPath;
}

QString PlaceholderUtility::getDeployEntryPointURL()
{
	return deployEntryPointURL;
}

QString PlaceholderUtility::getProvideServiceEndPointURL()
{
	return provideServiceEndPointURL;
}

bool PlaceholderUtility::isMachineConfiguredForVirtualBox()
{
		bool result = true;
		if( !exists(virtualBoxPath.toUtf8().constData()) ) {
			result = false;
		}
		else if( !exists((virtualBoxPath + virtualBoxManage).toUtf8().constData()) ) {
			result = false;
		}		
		return result;
}

QString PlaceholderUtility::getArtifactDetailURL()
{
	return artifactDetailURL;
}


QString PlaceholderUtility::getArtifactJson()
{
	return artifactJson;
}

QString PlaceholderUtility::getPaymentAddress()
{
	return paymentAddress;
}

QString PlaceholderUtility::getRegisterServiceEndPointURL()
{
	return registerServiceEndPointURL;
}

QString PlaceholderUtility::getVDIPath()
{
	return vdiPath;
}

QString PlaceholderUtility::getContentTypeByExtension(QString fileName) 
{
		QString contentMimeType = "";
		
		if( fileName.endsWith(".jpg", Qt::CaseInsensitive) ) { 
			contentMimeType = "image/jpeg";								
		}
        else if( fileName.endsWith(".class", Qt::CaseInsensitive) ) { 
			contentMimeType = "application/java-byte-code";								
		}   
		else if( fileName.endsWith(".gif", Qt::CaseInsensitive) ) { 
			contentMimeType = "image/gif";								
		}
		else if( fileName.endsWith(".gpg", Qt::CaseInsensitive) ) { 
			contentMimeType = "application/pgp-encrypted";								
		}		
		else if( fileName.endsWith(".png", Qt::CaseInsensitive) ) { 
			contentMimeType = "image/png";								
		}		
		else if( fileName.endsWith(".js", Qt::CaseInsensitive) ) { 
			contentMimeType = "text/javascript";								
		}	
		else if( fileName.endsWith(".json", Qt::CaseInsensitive) ) { 
			contentMimeType = "application/json";								
		}
		else if( fileName.endsWith(".mp3", Qt::CaseInsensitive) ) { 
			contentMimeType = "audio/mpeg";								
		}
		else if( fileName.endsWith(".mpeg", Qt::CaseInsensitive) ) { 
			contentMimeType = "video/mpeg";								
		}
		else if( fileName.endsWith(".xml", Qt::CaseInsensitive) ) { 
			contentMimeType = "text/xml";								
		}
		else if( fileName.endsWith(".txt", Qt::CaseInsensitive) ) { 
			contentMimeType = "text/plain";								
		}
		else if( fileName.endsWith(".pdf", Qt::CaseInsensitive) ) { 
			contentMimeType = "application/pdf";								
		}
		else if( fileName.endsWith(".zip", Qt::CaseInsensitive) ) { 
			contentMimeType = "application/zip";								
		}
		else if( fileName.endsWith(".jar", Qt::CaseInsensitive) ) { 
			contentMimeType = "application/java-archive";								
		}
		else if( fileName.endsWith(".html", Qt::CaseInsensitive) || fileName.endsWith(".htm", Qt::CaseInsensitive) ) { 
			contentMimeType = "text/html";								
		}
		else if( fileName.endsWith(".torrent", Qt::CaseInsensitive) ) { 
			contentMimeType = "application/x-bittorrent";								
		}
		
		
		/*
		if( fileName.endsWith(".zip", Qt::CaseInsensitive) ) { 
		
			QMessageBox msgBoxError;
			msgBoxError.setText("This zip will be converted to torrent for seeding (NOT WORKING YET)");
			msgBoxError.exec();										

		}
		else if( fileName.endsWith(".vdi", Qt::CaseInsensitive) ) { 
			QMessageBox msgBoxC;
			msgBoxC.setText("This VDI will be converted to torrent for seeding (NOT WORKING YET)");
			msgBoxC.exec();										
		
		}
		else if( fileName.endsWith(".mpeg", Qt::CaseInsensitive) ) { 
			QMessageBox msgBoxC;
			msgBoxC.setText("This MPEG will be converted to torrent for seeding (NOT WORKING YET)");
			msgBoxC.exec();										
		
		}
		else
		*/	
		
		return contentMimeType;
		
}


QString PlaceholderUtility::getExtensionByContentType(QString str)
{
	if( str == "application/x-bittorrent" ) { 
		return ".torrent";
	} else if( str == "image/jpeg" ) { 
		return ".jpg";
	} else if( str == "application/pgp-encrypted" ) { 
		return ".gpg";
	} else if( str == "image/gif" ) { 
		return ".gif";
	} else if( str == "image/png" ) { 
		return ".png";
	} else if( str == "application/pdf" ) { 
		return ".pdf";
	} else if( str == "audio/mpeg" ) { 
		return ".mp3";
	} else if( str == "text/html" ) { 
		return ".html";
	} else if( str == "text/plain" ) { 
		return ".txt";
	} else if( str == "text/xml" ) { 
		return ".xml";
	} else if( str == "text/javascript" ) { 
		return ".js";
	} else if( str == "application/json" ) { 
		return ".json";
	} else if( str == "application/x-virtualbox-vdi" ) { 
		return ".vdi";
	} else if( str == "application/java-archive" ) { 
		return ".jar";
	}  else if( str == "application/java-byte-code" ) { 
		return ".class";
	} 
	
	return ".artifact";
	
}

QJsonObject PlaceholderUtility::objectFromString(const QString& in)
{
    QJsonObject obj;

    QJsonDocument doc = QJsonDocument::fromJson(in.toUtf8());

    // check validity of the document
    if(!doc.isNull())
    {
        if(doc.isObject())
        {
            obj = doc.object();        
        }
        else
        {
            qDebug() << "Document is not an object" << endl;
        }
    }
    else
    {
        qDebug() << "Invalid JSON...\n" << in << endl;
    }

    return obj;
}

inline bool PlaceholderUtility::exists(const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}

void PlaceholderUtility::download(QString artifact)
{
	try { 
		QProcess process;
		QString torrentFile =  aria2cPath + pathSeperator + aria2cName + " --allow-overwrite --seed-time=0 --out=" + artifact + ".artifact --dir=" + vdiPath + " " + artifactSeedAnnounceURL + artifact;
		process.start(torrentFile);
		process.waitForFinished();
		process.close();	
	} catch(...) { 
		
	}
}

void PlaceholderUtility::seed(QString artifact)
{
	try { 
		QString strRatio = QString::number(0.25);
		QProcess process;
		QString torrentFile =  aria2cPath + pathSeperator + aria2cName + " --allow-overwrite --seed-ratio=" + strRatio + " --out=" + artifact + ".artifact --dir=" + vdiPath + " " + artifactSeedAnnounceURL + artifact;
		process.start(torrentFile);
		process.waitForFinished();
		process.close();	
	} catch(...) { 
	
	}
	
}

void PlaceholderUtility::seedRepository()
{
	try { 
		std::string line;
		std::string cd;
		cd = repositoryListFile.toUtf8().constData();
		
		
		
		std::ifstream myfile (cd);
		if (myfile.is_open())
		{
			while ( std::getline (myfile,line) )
			{
				try { 
					QString q = QString::fromLocal8Bit(line.c_str());
					seed(q);
				} catch(...) { } 
			}
			myfile.close();
		}
		else {
			QMessageBox msgBoxError;
			msgBoxError.setText("Unable to begin seeding from repository file list.json in {" + repositoryListFile + "}");
			msgBoxError.exec();
		}		
	} catch(...) { 
	
	}
}

int PlaceholderUtility::getNumberArtifacts()
{
	int num = 0;
	try { 
		std::string fcline;
		std::string cd;
		cd = repositoryListFile.toUtf8().constData();
		
		std::ifstream lineCountFile (cd);

		
		if(lineCountFile.is_open()){
			while(!lineCountFile.eof()){
				std::getline(lineCountFile,fcline);
				num++;
			}
			lineCountFile.close();
		}
	} catch(...) { 
	
	}
	
	return num;
}

