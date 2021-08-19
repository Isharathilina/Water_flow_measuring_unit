-- phpMyAdmin SQL Dump
-- version 4.2.7.1
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Aug 19, 2021 at 03:27 AM
-- Server version: 10.5.8-MariaDB
-- PHP Version: 5.4.31

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `test`
--

-- --------------------------------------------------------

--
-- Table structure for table `waterflowdata`
--

CREATE TABLE IF NOT EXISTS `waterflowdata` (
`id` int(6) NOT NULL,
  `rate` int(4) NOT NULL,
  `total` int(6) NOT NULL,
  `time` varchar(50) NOT NULL
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=12941 ;

--
-- Dumping data for table `waterflowdata`
--

INSERT INTO `waterflowdata` (`id`, `rate`, `total`, `time`) VALUES
(1, 0, 0, '8:15:42'),
(2, 0, 0, '8:15:42');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `waterflowdata`
--
ALTER TABLE `waterflowdata`
 ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `waterflowdata`
--
ALTER TABLE `waterflowdata`
MODIFY `id` int(6) NOT NULL AUTO_INCREMENT,AUTO_INCREMENT=12941;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
