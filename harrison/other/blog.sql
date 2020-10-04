/*
 Navicat Premium Data Transfer

 Source Server         : aliyun
 Source Server Type    : MySQL
 Source Server Version : 50729
 Source Host           : 101.37.35.34:23306
 Source Schema         : blog

 Target Server Type    : MySQL
 Target Server Version : 50729
 File Encoding         : 65001

 Date: 13/05/2020 07:57:44
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for archive
-- ----------------------------
DROP TABLE IF EXISTS `archive`;
CREATE TABLE `archive`  (
  `title` varchar(32) CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NOT NULL,
  `img_url` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin NULL DEFAULT NULL,
  `a_order` int(11) UNSIGNED NOT NULL,
  PRIMARY KEY (`title`) USING BTREE,
  INDEX `title`(`title`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_bin ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of archive
-- ----------------------------
INSERT INTO `archive` VALUES ('计算机组成原理', 'https://cdn.harrisonlee.net/1_QQn2VbwlEZDxtMYm53VdCQ.jpeg', 0);

-- ----------------------------
-- Table structure for article
-- ----------------------------
DROP TABLE IF EXISTS `article`;
CREATE TABLE `article`  (
  `id` char(12) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `title` varchar(128) CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NOT NULL,
  `markdown` text CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NOT NULL,
  `html` text CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NOT NULL,
  `star` int(11) NOT NULL DEFAULT 0,
  `view_times` int(11) NOT NULL DEFAULT 0,
  `is_open` int(1) NOT NULL DEFAULT 1 COMMENT '1表示公开，0表示隐私',
  `is_delete` int(1) NOT NULL DEFAULT 0 COMMENT '0表示为删除，1表示删除',
  `post_date` datetime(0) NOT NULL,
  `last_update_date` datetime(0) NOT NULL,
  `archive_title` varchar(32) CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NOT NULL,
  `is_reward` int(1) NOT NULL DEFAULT 0 COMMENT '1表示开启打赏，0表示关闭打赏',
  PRIMARY KEY (`id`) USING BTREE,
  INDEX `fk_archive_title`(`archive_title`) USING BTREE,
  CONSTRAINT `fk_archive_title` FOREIGN KEY (`archive_title`) REFERENCES `archive` (`title`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_bin ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of article
-- ----------------------------

-- ----------------------------
-- Table structure for blog
-- ----------------------------
DROP TABLE IF EXISTS `blog`;
CREATE TABLE `blog`  (
  `art_id` char(12) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `tag_id` char(12) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  PRIMARY KEY (`art_id`, `tag_id`) USING BTREE,
  INDEX `fk_b_t`(`tag_id`) USING BTREE,
  CONSTRAINT `fk_b_a` FOREIGN KEY (`art_id`) REFERENCES `article` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `fk_b_t` FOREIGN KEY (`tag_id`) REFERENCES `tag` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_bin ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of blog
-- ----------------------------

-- ----------------------------
-- Table structure for friend
-- ----------------------------
DROP TABLE IF EXISTS `friend`;
CREATE TABLE `friend`  (
  `id` char(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `title` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL COMMENT '网站的名称',
  `url` varchar(64) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `email` varchar(64) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_bin ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of friend
-- ----------------------------

-- ----------------------------
-- Table structure for menu
-- ----------------------------
DROP TABLE IF EXISTS `menu`;
CREATE TABLE `menu`  (
  `title` varchar(16) CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NOT NULL,
  `link` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `is_open` int(1) NOT NULL DEFAULT 1 COMMENT '1表示公开，0表示隐私',
  PRIMARY KEY (`title`, `link`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_bin ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of menu
-- ----------------------------

-- ----------------------------
-- Table structure for tag
-- ----------------------------
DROP TABLE IF EXISTS `tag`;
CREATE TABLE `tag`  (
  `id` char(12) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `title` varchar(32) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `create_date` datetime(0) NOT NULL,
  `view_times` int(11) UNSIGNED NOT NULL DEFAULT 0,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_bin ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of tag
-- ----------------------------

SET FOREIGN_KEY_CHECKS = 1;
