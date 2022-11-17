-- phpMyAdmin SQL Dump
-- version 5.2.0
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Nov 18, 2022 at 12:25 AM
-- Server version: 10.4.24-MariaDB
-- PHP Version: 8.1.6

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `sensor_data`
--

-- --------------------------------------------------------

--
-- Table structure for table `lab6_data`
--

CREATE TABLE `lab6_data` (
  `ID` int(11) NOT NULL,
  `MCU_ID` int(11) NOT NULL,
  `Light_Intensity` float NOT NULL,
  `Distance` float NOT NULL,
  `Time` timestamp(6) NOT NULL DEFAULT current_timestamp(6) ON UPDATE current_timestamp(6)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `lab6_data`
--

INSERT INTO `lab6_data` (`ID`, `MCU_ID`, `Light_Intensity`, `Distance`, `Time`) VALUES
(3, 1, 59, 78, '2022-11-17 22:02:02.032598'),
(4, 1, 59, 78, '2022-11-17 22:08:25.468986'),
(5, 1, 777, 888, '2022-11-17 22:10:32.644609'),
(6, 1, 777, 888, '2022-11-17 22:10:52.449406'),
(7, 1, 77798, 88888, '2022-11-17 22:11:33.613322'),
(8, 1, 7, 8, '2022-11-17 22:12:12.595392'),
(9, 1, 7778, 8907, '2022-11-17 22:12:35.955850'),
(10, 1, 76.8, 89.99, '2022-11-17 22:15:11.599286'),
(11, 1, 76.8, 89.99, '2022-11-17 22:55:07.490249'),
(12, 1, 56.8, 90.5, '2022-11-17 22:55:32.869356');

-- --------------------------------------------------------

--
-- Table structure for table `mcu`
--

CREATE TABLE `mcu` (
  `MCU_ID` int(4) NOT NULL,
  `Name` varchar(32) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `mcu`
--

INSERT INTO `mcu` (`MCU_ID`, `Name`) VALUES
(1, 'ESP1'),
(2, 'ESP2');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `lab6_data`
--
ALTER TABLE `lab6_data`
  ADD PRIMARY KEY (`ID`),
  ADD KEY `MCU_ID` (`MCU_ID`),
  ADD KEY `MCU_ID_2` (`MCU_ID`);

--
-- Indexes for table `mcu`
--
ALTER TABLE `mcu`
  ADD PRIMARY KEY (`MCU_ID`),
  ADD KEY `MCU_ID` (`MCU_ID`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `lab6_data`
--
ALTER TABLE `lab6_data`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=13;

--
-- AUTO_INCREMENT for table `mcu`
--
ALTER TABLE `mcu`
  MODIFY `MCU_ID` int(4) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=3;

--
-- Constraints for dumped tables
--

--
-- Constraints for table `lab6_data`
--
ALTER TABLE `lab6_data`
  ADD CONSTRAINT `lab6_data_ibfk_1` FOREIGN KEY (`MCU_ID`) REFERENCES `mcu` (`MCU_ID`) ON DELETE CASCADE ON UPDATE CASCADE;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
