-- MySQL dump 10.13  Distrib 5.7.42, for Linux (aarch64)
--
-- Host: localhost    Database: oil_cloud
-- ------------------------------------------------------
-- Server version	5.7.42-0ubuntu0.18.04.1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `Controller`
--

DROP TABLE IF EXISTS `Controller`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `Controller` (
  `ID` smallint(6) NOT NULL,
  `ControllerID` smallint(6) NOT NULL,
  `Controllertype` smallint(6) NOT NULL,
  `OutputVoltage` smallint(6) NOT NULL,
  `OutputCurrent` smallint(6) NOT NULL,
  `WellNum` smallint(6) NOT NULL,
  `datetime1` smallint(6) NOT NULL,
  `datetime2` smallint(6) NOT NULL,
  `datetime3` smallint(6) NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Controller`
--

LOCK TABLES `Controller` WRITE;
/*!40000 ALTER TABLE `Controller` DISABLE KEYS */;
INSERT INTO `Controller` VALUES (39,512,768,1024,1280,1536,0,0,0);
/*!40000 ALTER TABLE `Controller` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ControllerRecord`
--

DROP TABLE IF EXISTS `ControllerRecord`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ControllerRecord` (
  `ID` bigint(20) NOT NULL AUTO_INCREMENT,
  `DeviceID` smallint(6) NOT NULL,
  `ControllerID` smallint(6) NOT NULL,
  `Controllertype` smallint(6) NOT NULL,
  `OutputVoltage` smallint(6) NOT NULL,
  `OutputCurrent` smallint(6) NOT NULL,
  `WellNum` smallint(6) NOT NULL,
  `datetime1` smallint(6) NOT NULL,
  `datetime2` smallint(6) NOT NULL,
  `datetime3` smallint(6) NOT NULL,
  `Timestamp` timestamp(3) NOT NULL DEFAULT CURRENT_TIMESTAMP(3) ON UPDATE CURRENT_TIMESTAMP(3),
  PRIMARY KEY (`ID`),
  KEY `AddressID` (`DeviceID`),
  CONSTRAINT `ControllerRecord_ibfk_1` FOREIGN KEY (`DeviceID`) REFERENCES `Controller` (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=3353 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ControllerRecord`
--

LOCK TABLES `ControllerRecord` WRITE;
/*!40000 ALTER TABLE `ControllerRecord` DISABLE KEYS */;
/*!40000 ALTER TABLE `ControllerRecord` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `Mandrel`
--

DROP TABLE IF EXISTS `Mandrel`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `Mandrel` (
  `ID` smallint(6) NOT NULL,
  `WellID` smallint(6) NOT NULL,
  `Layer` smallint(6) NOT NULL,
  `Traffic` float NOT NULL,
  `Temperature` smallint(6) NOT NULL,
  `InternalPressure` smallint(6) NOT NULL,
  `ExternalPressure` smallint(6) NOT NULL,
  `OpeningAccuracy` smallint(6) NOT NULL,
  `MoisturePercentage` smallint(6) NOT NULL,
  `Status` smallint(6) NOT NULL,
  `MotorStatus` smallint(6) NOT NULL,
  `MotorCurrent` smallint(6) NOT NULL,
  `Voltage` smallint(6) NOT NULL,
  `Current` smallint(6) NOT NULL,
  `OperatingMode` smallint(6) NOT NULL,
  `ControlStatus` smallint(6) NOT NULL,
  `YieldControl` smallint(6) NOT NULL,
  `YieldValue` float NOT NULL,
  `UpFlowDeviation` smallint(6) NOT NULL,
  `DownFlowDeviation` smallint(6) NOT NULL,
  `AdjustmentMinutes` smallint(6) NOT NULL,
  `PressureTestControl` smallint(6) NOT NULL,
  `PressureTestHour` smallint(6) NOT NULL,
  `WaterAbsorptionTest` smallint(6) NOT NULL,
  `FaucetFlag` smallint(6) NOT NULL,
  PRIMARY KEY (`ID`),
  KEY `WellID` (`WellID`),
  CONSTRAINT `Mandrel_ibfk_1` FOREIGN KEY (`WellID`) REFERENCES `Well` (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Mandrel`
--

LOCK TABLES `Mandrel` WRITE;
/*!40000 ALTER TABLE `Mandrel` DISABLE KEYS */;
/*!40000 ALTER TABLE `Mandrel` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `MandrelRecord`
--

DROP TABLE IF EXISTS `MandrelRecord`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `MandrelRecord` (
  `ID` bigint(20) NOT NULL AUTO_INCREMENT,
  `DeviceID` smallint(6) NOT NULL,
  `WellID` smallint(6) NOT NULL,
  `Layer` smallint(6) NOT NULL,
  `Traffic` float NOT NULL,
  `Temperature` smallint(6) NOT NULL,
  `InternalPressure` smallint(6) NOT NULL,
  `ExternalPressure` smallint(6) NOT NULL,
  `OpeningAccuracy` smallint(6) NOT NULL,
  `MoisturePercentage` smallint(6) NOT NULL,
  `Status` smallint(6) NOT NULL,
  `MotorStatus` smallint(6) NOT NULL,
  `MotorCurrent` smallint(6) NOT NULL,
  `Voltage` smallint(6) NOT NULL,
  `Current` smallint(6) NOT NULL,
  `OperatingMode` smallint(6) NOT NULL,
  `ControlStatus` smallint(6) NOT NULL,
  `YieldControl` smallint(6) NOT NULL,
  `YieldValue` float NOT NULL,
  `UpFlowDeviation` smallint(6) NOT NULL,
  `DownFlowDeviation` smallint(6) NOT NULL,
  `AdjustmentMinutes` smallint(6) NOT NULL,
  `PressureTestControl` smallint(6) NOT NULL,
  `PressureTestHour` smallint(6) NOT NULL,
  `WaterAbsorptionTest` smallint(6) NOT NULL,
  `FaucetFlag` smallint(6) NOT NULL,
  `Timestamp` timestamp(3) NOT NULL DEFAULT CURRENT_TIMESTAMP(3) ON UPDATE CURRENT_TIMESTAMP(3),
  PRIMARY KEY (`ID`),
  KEY `AddressID` (`DeviceID`),
  KEY `WellID` (`WellID`),
  CONSTRAINT `MandrelRecord_ibfk_1` FOREIGN KEY (`DeviceID`) REFERENCES `Mandrel` (`ID`),
  CONSTRAINT `MandrelRecord_ibfk_2` FOREIGN KEY (`WellID`) REFERENCES `Well` (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=16265 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `MandrelRecord`
--

LOCK TABLES `MandrelRecord` WRITE;
/*!40000 ALTER TABLE `MandrelRecord` DISABLE KEYS */;
/*!40000 ALTER TABLE `MandrelRecord` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `Status_Alarm`
--

DROP TABLE IF EXISTS `Status_Alarm`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `Status_Alarm` (
  `AlarmID` bigint(20) NOT NULL AUTO_INCREMENT,
  `Timestamp` timestamp(3) NOT NULL DEFAULT CURRENT_TIMESTAMP(3) ON UPDATE CURRENT_TIMESTAMP(3),
  `DeviceID` smallint(6) NOT NULL,
  `Reason` varchar(200) NOT NULL,
  PRIMARY KEY (`AlarmID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Status_Alarm`
--

LOCK TABLES `Status_Alarm` WRITE;
/*!40000 ALTER TABLE `Status_Alarm` DISABLE KEYS */;
/*!40000 ALTER TABLE `Status_Alarm` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `User`
--

DROP TABLE IF EXISTS `User`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `User` (
  `UserID` bigint(20) NOT NULL,
  `Username` varchar(20) NOT NULL,
  `Password` varchar(20) NOT NULL,
  `Department` varchar(20) DEFAULT NULL,
  `Phone` varchar(20) DEFAULT NULL,
  PRIMARY KEY (`UserID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `User`
--

LOCK TABLES `User` WRITE;
/*!40000 ALTER TABLE `User` DISABLE KEYS */;
INSERT INTO `User` VALUES (1,'123','123','1','123'),(2,'lucky','1234','cehua','123456'),(3,'lucky','123','depart','123456');
/*!40000 ALTER TABLE `User` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `UserAction`
--

DROP TABLE IF EXISTS `UserAction`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `UserAction` (
  `ActionID` smallint(6) NOT NULL,
  `UserID` bigint(6) NOT NULL,
  `ActionDetails` varchar(200) NOT NULL,
  `Timestamp` timestamp(3) NOT NULL DEFAULT CURRENT_TIMESTAMP(3) ON UPDATE CURRENT_TIMESTAMP(3),
  PRIMARY KEY (`ActionID`),
  KEY `UserID` (`UserID`),
  CONSTRAINT `UserAction_ibfk_1` FOREIGN KEY (`UserID`) REFERENCES `User` (`UserID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `UserAction`
--

LOCK TABLES `UserAction` WRITE;
/*!40000 ALTER TABLE `UserAction` DISABLE KEYS */;
/*!40000 ALTER TABLE `UserAction` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `Well`
--

DROP TABLE IF EXISTS `Well`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `Well` (
  `ID` smallint(6) NOT NULL,
  `WellID` smallint(6) NOT NULL,
  `WellName` varchar(40) NOT NULL,
  `LayerNumber` smallint(6) NOT NULL,
  `PowerStatus` smallint(6) NOT NULL,
  `StatusFlag` smallint(6) NOT NULL,
  `OperatingMode` smallint(6) NOT NULL,
  `ControlTraffic` float NOT NULL,
  `ControlPressure` smallint(6) NOT NULL,
  `ControlOpening` smallint(6) NOT NULL,
  `ControlStatus` smallint(6) NOT NULL,
  `TrafficSetting` float NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Well`
--

LOCK TABLES `Well` WRITE;
/*!40000 ALTER TABLE `Well` DISABLE KEYS */;
/*!40000 ALTER TABLE `Well` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `WellRecord`
--

DROP TABLE IF EXISTS `WellRecord`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `WellRecord` (
  `ID` bigint(20) NOT NULL AUTO_INCREMENT,
  `DeviceID` smallint(6) NOT NULL,
  `WellID` smallint(6) NOT NULL,
  `WellName` varchar(40) NOT NULL,
  `LayerNumber` smallint(6) NOT NULL,
  `PowerStatus` smallint(6) NOT NULL,
  `StatusFlag` smallint(6) NOT NULL,
  `OperatingMode` smallint(6) NOT NULL,
  `ControlTraffic` float NOT NULL,
  `ControlPressure` smallint(6) NOT NULL,
  `ControlOpening` smallint(6) NOT NULL,
  `ControlStatus` smallint(6) NOT NULL,
  `TrafficSetting` float NOT NULL,
  `Timestamp` timestamp(3) NOT NULL DEFAULT CURRENT_TIMESTAMP(3) ON UPDATE CURRENT_TIMESTAMP(3),
  PRIMARY KEY (`ID`),
  KEY `AddressID` (`DeviceID`),
  CONSTRAINT `WellRecord_ibfk_1` FOREIGN KEY (`DeviceID`) REFERENCES `Well` (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=10286 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `WellRecord`
--

LOCK TABLES `WellRecord` WRITE;
/*!40000 ALTER TABLE `WellRecord` DISABLE KEYS */;
/*!40000 ALTER TABLE `WellRecord` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2023-10-20 22:34:46
