<?
//  Pingus Level Feedback system
//  Copyright (C) 2003 by Jarno Elonen <elonen@iki.fi>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

require_once("xml-search.inc");

global $db_handle;
$db_handle = False;

function open_cache()
{
  global $db_handle;
  if ( !$db_handle )
  {
    $db_driver = "db3";
    //$db_driver = "db4";

    $dbfile = "/home/elonen/public_html/pingus/comments/cache.db";
    //$dbfile = "/var/www/pingus/comments/cache.db";

    $db_handle = @dba_open ($dbfile, "w", $db_driver);
    if (!$db_handle)
      $db_handle = dba_open ($dbfile, "c", $db_driver);
    if (!$db_handle)
    {
      echo '<p><font color="red"><b>ERROR: Cannot open cache file!</b></font></p>';
      exit;
    }
  }
}

function close_cache()
{
  global $db_handle;
  if ( $db_handle !== False )
  {
    dba_close($db_handle);
    $db_handle = False;
  }
}

function make_db_key( $cathegory, $level )
{
  return $cathegory . "//" . $level;
}

function level_cache_get( $cathegory, $level )
{
  global $db_handle;
  open_cache();
  $data = dba_fetch( make_db_key( $cathegory, $level ), $db_handle );
  if ( $data === False )
  {
    $levelfile = sandbox_check( "data/levels/$cathegory/$level.plf", "data/" );
    $leveldata = parse_level( $levelfile );
    $leveldata["md5sum"] = md5(implode("", file($levelfile)));

    $comments = parse_level_comments($cathegory, $level);
    $avgrating = 0;
    if (count($comments) > 0 )
    {
      while( list(,$v) = each($comments))
        $avgrating += intval($v["rating"]);
      $avgrating /= count($comments);
    }
    $leveldata["avgrating"] = $avgrating;
    $leveldata["totalcomments"] = count($comments);

    level_cache_save( $cathegory, $level, $leveldata );
    return $leveldata;
  }
  $ret = Array();
  $arr = explode("\255", $data);
  while (list(,$fld) = each($arr))
  {
    if ( strlen($fld))
    {
      list($k,$v) = explode("=", $fld, 2);
      $ret[$k] = $v;
    }
  }
  unset( $data );
  return $ret;
}

function level_cache_del( $cathegory, $level )
{
  global $db_handle;
  open_cache();
  if ( !dba_delete(make_db_key( $cathegory, $level ), $db_handle))
  {
    echo '<p><font color="red"><b>ERROR: Cannot delete cache key!</b></font></p>';
    return False;
  }
  else return True;
}

function level_cache_save( $cathegory, $level, $arr )
{
  global $db_handle;
  open_cache();
  $data = "";
  reset( $arr );
  while (list($k,$v) = each($arr))
    $data .= $k . "=" . $v . "\255";
  if (!dba_replace( make_db_key( $cathegory, $level ), $data, $db_handle ))
  {
    echo '<p><font color="red"><b>ERROR: Cannot save level cache entry!</b></font></p>';
    return False;
  }
  else return True;
}

function parse_level( $filename )
{
  $searches = Array(
    "name" => "pingus-level/global/levelname@lang=en",
    "description" => "pingus-level/global/description@lang=en",
    "comment" => "pingus-level/global/comment",
    "author" => "pingus-level/global/author",
    "pingusmin" => "pingus-level/global/number-to-save",
    "pingusmax" => "pingus-level/global/number-of-pingus",
    "time" => "pingus-level/global/time",
    "difficulty" => "pingus-level/global/difficulty",
    "playable" => "pingus-level/global/playable"
    );
  $actions = Array(
    "basher", "blocker", "bomber", "bridger", "climber",
    "digger", "floater", "jumper", "miner", "slider" );
  foreach( $actions as $a )
    $searches["#$a"] = "pingus-level/action-list/$a";

  $arr = Array();
  $res = xml_search( $filename, $searches );
  while( list($k,$v) = each($res))
  {
    if ( substr($k,0,1) == "#" )
      $arr[$k] = $v["a"]["COUNT"];
    else
      $arr[$k] = $v["v"];
      //$arr[$k] = decode_html($v["v"]);
  }
  unset( $res );
  return $arr;
}

function parse_level_comments( $cathegory, $level )
{
  $res = Array();
  $comment_dir = "comments/$cathegory/$level";
  if ( !is_dir( $comment_dir ))
    return $res;
  sandbox_check($comment_dir, "comments/");

  $dir = dir($comment_dir);
  $comments = Array();
  while ($file = $dir->read())
    if ($file != "." && $file != ".." && file_exists("$comment_dir/$file"))
      $comments[] = "$comment_dir/$file";
  $dir->close();

  while( list($key, $filename) = each($comments))
  {
    $cmt = xml_search( $filename, Array(
      "author" => "pingus-level-comment/author",
      "email" => "pingus-level-comment/email",
      "date" => "pingus-level-comment/date",
      "difficulty" => "pingus-level-comment/difficulty",
      "rating" => "pingus-level-comment/rating",
      "comment" => "pingus-level-comment/comment",
      "levelmd5" => "pingus-level-comment/levelmd5"
    ));

    // We don't need tag names and attributes, strip them off:
    while( list($k,$v) = each($cmt))
      $cmt[$k] = $v["v"];

    $cmt["filename"] = $filename;
    $res[] = $cmt;
  }
  return $res;
}

function sandbox_check($filename, $sandbox)
{
  $file = realpath( $filename );
  $sandbox = realpath( $sandbox );
  if ( strlen($file) < strlen($sandbox) || substr($file, 0, strlen($sandbox)) !== $sandbox )
    die( "<b>TRIED TO ACCESS A FILE OUTSIDE SANDBOX: '" .
      htmlentities($filename) . "'! ABORTING!</b>");
  return $filename;
}

?>
