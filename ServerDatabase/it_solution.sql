-- phpMyAdmin SQL Dump
-- version 4.8.5
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Apr 15, 2019 at 05:15 PM
-- Server version: 10.1.38-MariaDB
-- PHP Version: 7.2.16

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `it_solution`
--

-- --------------------------------------------------------



-- --------------------------------------------------------

--
-- Table structure for table `error_noti`
--


-- --------------------------------------------------------

--
-- Table structure for table `iot_home`
--

CREATE TABLE `iot_home` (
  `id` int(11) NOT NULL,
  `IDNODE` varchar(100) COLLATE utf8_vietnamese_ci NOT NULL,
  `TEMP` float NOT NULL,
  `HUMI` float NOT NULL,
  `GAS` float NOT NULL,
  `RAIN` float NOT NULL,
  `COMMAND` int(11) NOT NULL,
  `STATUS` varchar(100) COLLATE utf8_vietnamese_ci NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_vietnamese_ci;

--
-- Dumping data for table `iot_home`
--

INSERT INTO `iot_home` (`id`, `IDNODE`, `TEMP`, `HUMI`, `GAS`, `RAIN`, `COMMAND`, `STATUS`) VALUES
(1, 'CC', 0, 0, 0, 0, 0, 'Door Closed Light Off'),
(2, 'CN', 0, 0, 0, 0, 0, 'Door Closed Light Off'),
(3, 'NB', 0, 0, 0, 0, 0, 'Door Closed Light Off Fan Off'),
(4, 'NK', 0, 0, 0, 0, 0, 'Light Off Fan Off'),
(5, 'PN', 0, 0, 0, 0, 0, 'Light Off Fan Off'),
(6, 'PT', 0, 0, 0, 0, 0, 'Light Off'),
(7, 'PD', 0, 0, 0, 0, 0, 'Door Closed'),
(8, 'GR', 0, 0, 0, 0, 0, 'Door Closed');

-- --------------------------------------------------------


--
-- Table structure for table `user`
--

CREATE TABLE `user` (
  `id` int(11) NOT NULL,
  `USERNAME` varchar(100) COLLATE utf8_vietnamese_ci NOT NULL,
  `PASSWORD` varchar(100) COLLATE utf8_vietnamese_ci NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_vietnamese_ci;

--
-- Dumping data for table `user`
--

INSERT INTO `user` (`id`, `USERNAME`, `PASSWORD`) VALUES
(1, 'ITSOLUTION', '12345678');

--
-- Indexes for dumped tables
--



-- Indexes for table `iot_home`
--
ALTER TABLE `iot_home`
  ADD PRIMARY KEY (`id`);

--

-- Indexes for table `user`
--
ALTER TABLE `user`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT for dumped tables
--

--


--

--


--
-- AUTO_INCREMENT for table `iot_home`
--
ALTER TABLE `iot_home`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=9;

--

--
-- AUTO_INCREMENT for table `user`
--
ALTER TABLE `user`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
