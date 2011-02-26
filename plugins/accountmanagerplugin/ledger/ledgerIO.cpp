#include "ledgerIO.h"

#include <QDate>
#include <QDebug>

//todo : verify system of filter of accountmodel

LedgerIO::LedgerIO(QObject * parent){
    qDebug() << __FILE__ << QString::number(__LINE__) << " IO constructeur 1" ;
    if ((m_accountModel = new AccountModel(this)) == NULL)
    {
    	  qWarning() << __FILE__ << QString::number(__LINE__) << "AccountModel is null" ;
        }
    if ((m_movementModel = new MovementModel(this))== NULL)
    {
    	  qWarning() << __FILE__ << QString::number(__LINE__) << "MovementModel is null" ;
        }
    qDebug() << __FILE__ << QString::number(__LINE__) << " IO constructeur 2" ;
}

LedgerIO::~LedgerIO(){}

QStringList LedgerIO::getListOfYears(){
    QStringList list;
    int rows = m_accountModel->rowCount();
    for (int i = 0; i < rows; i += 1)
    {
    	QString date = m_accountModel->data(m_accountModel->index(i,ACCOUNT_DATE),Qt::DisplayRole).toString();
    	QDate dateYear = QDate::fromString(date,"yyyy");
    	list << dateYear.toString("yyyy");
    }
    qDebug() << __FILE__ << QString::number(__LINE__) << " listOfYears =" << QString::number(list.size()) ;
    list.removeDuplicates();
    return list;
}
    

AccountModel * LedgerIO::getModelMonthlyReceiptsIO(QObject * parent,QString & month , QString & year){
    QString dateBeginStr = year+"-"+month+"-01";
    QDate dateBegin = QDate::fromString(dateBeginStr,"yyyy-MM-dd");
    QDate dateMonthAfter;
    if (month.toInt() > 12)
    {
    	   dateMonthAfter = dateBegin.addMonths(1);
        }
    QString dateEndStr = year+"-"+month+"-"+QString::number(dateBegin.daysTo(dateMonthAfter));
    QString filter = QString("DATE BETWEEN '%1' AND '%2'").arg(dateBeginStr,dateEndStr);
    m_accountModel->setFilter(filter);
    return m_accountModel;
}



QStringList LedgerIO::getTypesByMonth(QString & month,QString & year){
    QStringList list;
    QString dateBeginStr = year+"-"+month+"-01";
    QDate dateBegin = QDate::fromString(dateBeginStr,"yyyy-MM-dd");
    QDate dateMonthAfter;
    if (month.toInt() > 12)
    {
    	   dateMonthAfter = dateBegin.addMonths(1);
        }
    QString dateEndStr = year+"-"+month+"-"+QString::number(dateBegin.daysTo(dateMonthAfter));
    QString filter = QString("DATE BETWEEN '%1' AND '%2'").arg(dateBeginStr,dateEndStr);
    m_accountModel->setFilter(filter);
    int rowsNumber = m_accountModel->rowCount();
    for (int i = 0; i < rowsNumber; i += 1)
    {
    	QString dataType = m_accountModel->data(m_accountModel->index(i,ACCOUNT_MEDICALPROCEDURE_TEXT),Qt::DisplayRole)
    	                   .toString();
    	if (dataType.contains("+"))
    	{
    		  list << dataType.split("+");
    	    }
    	else{
    	          list << dataType;
    	}
    }
    list.removeDuplicates();
    return list;
}

int LedgerIO::getNbrOfRowsByTypeAndMonth(QString & month,QString & year,QString & type){
    int rows = 0;
    QString dateBeginStr = year+"-"+month+"-01";
    QDate dateBegin = QDate::fromString(dateBeginStr,"yyyy-MM-dd");
    QDate dateMonthAfter;
    if (month.toInt() > 12)
    {
    	   dateMonthAfter = dateBegin.addMonths(1);
        }
    QString dateEndStr = year+"-"+month+"-"+QString::number(dateBegin.daysTo(dateMonthAfter));
    QString filter = QString("DATE BETWEEN '%1' AND '%2'").arg(dateBeginStr,dateEndStr);
    m_accountModel->setFilter(filter);
    int rowsNumber = m_accountModel->rowCount();
    for (int i = 0; i < rowsNumber; i += 1)
    {
    	QString dataType = m_accountModel->data(m_accountModel->index(i,ACCOUNT_MEDICALPROCEDURE_TEXT),Qt::DisplayRole)
    	                  .toString();
    	if (dataType.contains("+"))
    	{
    		  QStringList list = dataType.split("+");
    		  QString str;
    		  foreach(str,list){
    		      if (str == type)
    		      {
    		      	  rows++;
    		          }
    		      }
    	    }
    	else{
    	    if (dataType == type)
    	    {
    	    	  rows++;
    	        }
    	}
    }
    return rows;
}

QStringList LedgerIO::listOfTypesByYear(QString & year){
    QStringList list;
    QString dateBeginStr = year+"-01-01";
    QString dateEndStr = year+"-12-31";
    QString filter = QString("DATE BETWEEN '%1' AND '%2'").arg(dateBeginStr,dateEndStr);
    m_accountModel->setFilter(filter);
    int rowsNumber = m_accountModel->rowCount();
    for (int i = 0; i < rowsNumber; i += 1)
    {
    	QString dataType = m_accountModel->data(m_accountModel->index(i,ACCOUNT_MEDICALPROCEDURE_TEXT),Qt::DisplayRole)
    	                   .toString();
    	if (dataType.contains("+"))
    	{
    		  list << dataType.split("+");
    	    }
    	else{
    	          list << dataType;
    	}
    }
    list.removeDuplicates();
    return list;
}
int LedgerIO::getNbrOfRowsByTypeAndYear(QString & year,QString & type){
    QStringList list;
    int rows = 0;
    QString dateBeginStr = year+"-01-01";
    QString dateEndStr = year+"-12-31";
    QString filter = QString("DATE BETWEEN '%1' AND '%2'").arg(dateBeginStr,dateEndStr);
    m_accountModel->setFilter(filter);
    int rowsNumber = m_accountModel->rowCount();
    for (int i = 0; i < rowsNumber; i += 1)
    {
    	QString dataType = m_accountModel->data(m_accountModel->index(i,ACCOUNT_MEDICALPROCEDURE_TEXT),Qt::DisplayRole)
    	                   .toString();
    	if (dataType.contains("+"))
    	{
    		  QStringList list = dataType.split("+");
    		  QString str;
    		  foreach(str,list){
    		      if (str == type)
    		      {
    		      	  rows++;
    		          }
    		      }
    	    }
    	else{
    	    if (dataType == type)
    	    {
    	    	  rows++;
    	        }
    	}
    }
    return rows;

}

double LedgerIO::getYearlyReceiptsSum(QObject * parent,QString & year){
    double totalValue = 0.00;
    QString dateBeginStr = year+"-01-01";
    QString dateEndStr = year+"-12-31";
    QString filter = QString("DATE BETWEEN '%1' AND '%2'").arg(dateBeginStr,dateEndStr);
    m_accountModel->setFilter(filter);
    int rows = 0;
    rows = m_accountModel->rowCount();    
    for (int i = 0; i < rows; i += 1)
    {
    	totalValue += m_accountModel->data(m_accountModel->index(i,ACCOUNT_CASHAMOUNT),Qt::DisplayRole).toDouble();
    	totalValue += m_accountModel->data(m_accountModel->index(i,ACCOUNT_CHEQUEAMOUNT),Qt::DisplayRole).toDouble();
    	totalValue += m_accountModel->data(m_accountModel->index(i,ACCOUNT_VISAAMOUNT),Qt::DisplayRole).toDouble();
    	totalValue += m_accountModel->data(m_accountModel->index(i,ACCOUNT_INSURANCEAMOUNT),Qt::DisplayRole).toDouble();
    	totalValue += m_accountModel->data(m_accountModel->index(i,ACCOUNT_OTHERAMOUNT),Qt::DisplayRole).toDouble();    	
    }
    return totalValue;
}

MovementModel * LedgerIO::getModelMonthlyMovementsIO(QObject * parent,QString & month, QString & year){
    QString dateBeginStr = year+"-"+month+"-01";
    QDate dateBegin = QDate::fromString(dateBeginStr,"yyyy-MM-dd");
    QDate dateMonthAfter;
    if (month.toInt() > 12)
    {
    	   dateMonthAfter = dateBegin.addMonths(1);
        }
    QString dateEndStr = year+"-"+month+"-"+QString::number(dateBegin.daysTo(dateMonthAfter));
    QString filter = QString("DATE BETWEEN '%1' AND '%2'").arg(dateBeginStr,dateEndStr);
    m_movementModel->setFilter(filter);
    return m_movementModel;
}

double LedgerIO::getMovementSum(MovementModel * model){
    double value = 0.00;
    int rows = model->rowCount();
    for (int i = 0; i < rows; i += 1)
    {
    	value += model->data(model->index(i,MOV_AMOUNT),Qt::DisplayRole).toDouble();
    }
    return value;
}

QStandardItemModel * LedgerIO::getModelMonthlyAndTypeMovementsIO(QObject * parent,QString & month, QString & year){
    QStandardItemModel * model = new QStandardItemModel(this);
    QHash<QString,double> hash;
    int rows = 0;
    QString dateBeginStr = year+"-"+month+"-01";
    QDate dateBegin = QDate::fromString(dateBeginStr,"yyyy-MM-dd");
    QDate dateMonthAfter;
    if (month.toInt() > 12)
    {
    	   dateMonthAfter = dateBegin.addMonths(1);
        }
    QString dateEndStr = year+"-"+month+"-"+QString::number(dateBegin.daysTo(dateMonthAfter));
    QString filter = QString("DATE BETWEEN '%1' AND '%2'").arg(dateBeginStr,dateEndStr);
    m_movementModel->setFilter(filter);
    rows = m_movementModel->rowCount();
    for (int i = 0; i < rows; i += 1)
    {
    	QString labelOfMovement = m_movementModel->data(m_movementModel->index(i,MOV_LABEL),Qt::DisplayRole).toString();
    	double value = m_movementModel->data(m_movementModel->index(i,MOV_AMOUNT),Qt::DisplayRole).toDouble();
    	hash.insertMulti(labelOfMovement,value);
    }
    QStringList keysList = hash.uniqueKeys();
    QString keyStr;
    foreach(keyStr,keysList){
        double valueAssocWithKey = 0.00;
        QList<double> valuesList = hash.values(keyStr);
        for (int i = 0; i < valuesList.size(); i += 1)
        {
        	valueAssocWithKey += valuesList[i];
        }
        QStandardItem * itemLabel = new QStandardItem(keyStr);
        QStandardItem * itemAmount = new QStandardItem(QString::number(valueAssocWithKey));
        QList<QStandardItem*> items;
        items << itemLabel << itemAmount;
        model->appendRow(items);
    }
    return model;
}

QStandardItemModel * LedgerIO::getModelYearlyAndTypeMovementsIO(QObject * parent,QString & year){
    QStandardItemModel * model = new QStandardItemModel(this);
    QHash<QString,double> hash;
    int rows = 0;
    QString dateBeginStr = year+"-01-01";
    QString dateEndStr = year+"-12-31";
    QString filter = QString("DATE BETWEEN '%1' AND '%2'").arg(dateBeginStr,dateEndStr);
    m_movementModel->setFilter(filter);
    rows = m_movementModel->rowCount();
    for (int i = 0; i < rows; i += 1)
    {
    	QString labelOfMovement = m_movementModel->data(m_movementModel->index(i,MOV_LABEL),Qt::DisplayRole).toString();
    	double value = m_movementModel->data(m_movementModel->index(i,MOV_AMOUNT),Qt::DisplayRole).toDouble();
    	hash.insertMulti(labelOfMovement,value);    	
    }
    QStringList keysList = hash.uniqueKeys();
    QString keyStr;
    foreach(keyStr,keysList){
        double valueAssocWithKey = 0.00;
        QList<double> valuesList = hash.values(keyStr);
        for (int i = 0; i < valuesList.size(); i += 1)
        {
        	valueAssocWithKey += valuesList[i];
        }
        QStandardItem * itemLabel = new QStandardItem(keyStr);
        QStandardItem * itemAmount = new QStandardItem(QString::number(valueAssocWithKey));
        QList<QStandardItem*> items;
        items << itemLabel << itemAmount;
        model->appendRow(items);
    }
    return model;
}
//for ledger
QStringList LedgerIO::getListOfSumsMonthlyReceiptsIO(QObject * parent,QString & dateBegin , QString & dateEnd){
    QStringList list;
    QString dateBeginStr = dateBegin;
    QString dateEndStr = dateEnd;
    QString filter = QString("DATE BETWEEN '%1' AND '%2'").arg(dateBeginStr,dateEndStr);
    m_accountModel->setFilter(filter);
    double cash = 0.00;
    double checks = 0.00;
    double creditCard = 0.00;
    double banking = 0.00;
    int rowsNumber = m_accountModel->rowCount();
    for (int i = 0; i < rowsNumber; i += 1)
    {
    	cash        += m_accountModel->data(m_accountModel->index(i,ACCOUNT_CASHAMOUNT),Qt::DisplayRole).toDouble();
    	checks      += m_accountModel->data(m_accountModel->index(i,ACCOUNT_CHEQUEAMOUNT),Qt::DisplayRole).toDouble();
    	creditCard  += m_accountModel->data(m_accountModel->index(i,ACCOUNT_VISAAMOUNT),Qt::DisplayRole).toDouble();
    	banking     += m_accountModel->data(m_accountModel->index(i,ACCOUNT_INSURANCEAMOUNT),Qt::DisplayRole).toDouble();
    }
    list << QString::number(cash) 
         << QString::number(checks) 
         << QString::number(creditCard) 
         << QString::number(banking);
    return list;
}

QStringList LedgerIO::getListOfSumsMonthlyMovementsIO(QObject * parent,QString & dateBegin , QString & dateEnd){
    QStringList list;
    QHash<QString,double> hash;
    int rows = 0;
    QString dateBeginStr = dateBegin;
    QString dateEndStr = dateEnd;
    QString filter = QString("DATE BETWEEN '%1' AND '%2'").arg(dateBeginStr,dateEndStr);
    m_movementModel->setFilter(filter);
    rows = m_movementModel->rowCount();
    for (int i = 0; i < rows; i += 1)
    {
    	QString labelOfMovement = m_movementModel->data(m_movementModel->index(i,MOV_LABEL),Qt::DisplayRole).toString();
    	double value = m_movementModel->data(m_movementModel->index(i,MOV_AMOUNT),Qt::DisplayRole).toDouble();
    	hash.insertMulti(labelOfMovement,value);
    }
    QStringList keysList = hash.uniqueKeys();
    QString keyStr;
    foreach(keyStr,keysList){
        double valueAssocWithKey = 0.00;
        QList<double> valuesList = hash.values(keyStr);
        for (int i = 0; i < valuesList.size(); i += 1)
        {
        	valueAssocWithKey += valuesList[i];
        }
        QString strPair = keyStr+"="+QString::number(valueAssocWithKey);
        list << strPair;
    }
    return list;
}

QStringList LedgerIO::listOfReceiptsTypes(){
    QStringList list;
    int rowsNumber = m_accountModel->rowCount();
    for (int i = 0; i < rowsNumber; i += 1)
    {
    	QString type = m_accountModel->data(m_accountModel->index(i,ACCOUNT_MEDICALPROCEDURE_TEXT),Qt::DisplayRole)
    	               .toString();
    	list << type;
    }
    return list;
}

QStringList LedgerIO::listOfMovementsTypes(){
    QStringList list;
    int rowsNumber = m_movementModel->rowCount();
    for (int i = 0; i < rowsNumber; i += 1)
    {
    	QString type = m_movementModel->data(m_movementModel->index(i,MOV_LABEL),Qt::DisplayRole)
    	               .toString();
    	list << type;
    }
    return list;    
}

QList<QVector<QString> > LedgerIO::getDatasReceitsInVector(QString & dateBegin,QString & dateEnd){
    QList<QVector<QString> > tableLedgerMonth;
    QString filter = QString("DATE BETWEEN '%1' AND '%2'").arg(dateBegin,dateEnd);
    m_accountModel->setFilter(filter);
    int rows = m_accountModel->rowCount();
    for (int i = 0; i < rows; i += 1)
    {
    	QString vectorDate = m_accountModel->data(m_accountModel->index(i,ACCOUNT_DATE),Qt::DisplayRole).toString();
        QString vectorPatient = m_accountModel->data(m_accountModel->index(i,ACCOUNT_PATIENT_NAME),Qt::DisplayRole)
                                                                                                     .toString();       
        QString vectorCash = m_accountModel->data(m_accountModel->index(i,ACCOUNT_CASHAMOUNT),Qt::DisplayRole)
    	                                                                                        .toString();
        QString vectorChecks = m_accountModel->data(m_accountModel->index(i,ACCOUNT_CHEQUEAMOUNT),Qt::DisplayRole)
                                                                                                   .toString();
        QString vectorVisa = m_accountModel->data(m_accountModel->index(i,ACCOUNT_VISAAMOUNT),Qt::DisplayRole).toString();
        QString vectorBanking = m_accountModel->data(m_accountModel->index(i,ACCOUNT_INSURANCEAMOUNT),Qt::DisplayRole)
                                                                                                         .toString();
        QString vectorActs = m_accountModel->data(m_accountModel->index(i,ACCOUNT_MEDICALPROCEDURE_TEXT),Qt::DisplayRole)
    	                                                                                         .toString();//acts
        QVector<QString> vector;
        vector << vectorDate 
               << vectorPatient 
               << vectorCash 
               << vectorChecks 
               << vectorVisa 
               << vectorBanking 
               << vectorActs;
        tableLedgerMonth << vector;
    }
//on a le tableau date,patient,esp,chq,cb,virement,acte du mois
    return tableLedgerMonth;
}

QList<QVector<QString> > LedgerIO::getDatasMovementsInVector(QString & dateBegin,QString & dateEnd){
    QList<QVector<QString> > tableLedgerMonth;
    QString filter = QString("DATE BETWEEN '%1' AND '%2'").arg(dateBegin,dateEnd);
    m_movementModel->setFilter(filter);
    int rows = m_movementModel->rowCount();
    for (int i = 0; i < rows; i += 1)
    {
    	QString vectorDate = m_movementModel->data(m_movementModel->index(i,MOV_DATE),Qt::DisplayRole).toString();
        QString vectorLabel = m_movementModel->data(m_movementModel->index(i,MOV_LABEL),Qt::DisplayRole).toString();
        QString vectorAmount = m_movementModel->data(m_movementModel->index(i,MOV_AMOUNT),Qt::DisplayRole).toString();
        QString vectorComment = m_movementModel->data(m_movementModel->index(i,MOV_COMMENT),Qt::DisplayRole).toString();
        QString vectorDetails = m_movementModel->data(m_movementModel->index(i,MOV_DETAILS),Qt::DisplayRole).toString();
        QVector<QString> vector;
        vector << vectorDate << vectorLabel << vectorAmount << vectorComment << vectorDetails;
        tableLedgerMonth << vector;
    }
    return tableLedgerMonth;
}

