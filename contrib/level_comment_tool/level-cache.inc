<?
//  Pingus Level Feedback system
//  Copyright (C) 2003 by Jarno Elonen <elonen@iki.fi>
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
// + Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
// + Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// + The name of the author may not be used to endorse or promote products derived
//   from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ''AS IS'' AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR
// BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

require_once("xml-search.inc");

// ==================================================================
// Creates a table if necessary and returns the tablename
// ==================================================================
function create_table( $conn, $db, $tablename, $SQL )
{
  $table_exists = False;
  $res = mysql_list_tables( $db );

  for ($i=0;$i<mysql_num_rows( $res );$i++)
    if (mysql_tablename( $res, $i ) == $tablename)
      $table_exists = True;

  if (!$table_exists)
  {
    $res = mysql_query( $SQL, $conn );
    if (!$res)
      return False;
    else
      return $tablename;
  }
  else
    return $tablename;
}


function open_cache()
{
  global $DBMS;
  global $db_handle;
  global $db_tablename;
  global $db_name;
  global $db_host;
  global $db_user;
  global $db_pass;

  if ( !$db_handle )
  {
    switch( $DBMS )
    {
	  //DBA is used as cache
      case "DBA":
        $db_driver = "db3";

        $dbfile = "/home/pingus/public_html/level_comment_tool/comments/cache.db";
        //$dbfile = "/var/www/pingus/comments/cache.db";

        $db_handle = @dba_open ($dbfile, "w", $db_driver);
        if (!$db_handle)
          $db_handle = dba_open ($dbfile, "c", $db_driver);
        if (!$db_handle)
        {
          echo '<p><font color="red"><b>ERROR: Cannot open cache file!</b></font></p>';
          exit;
        }
        break;

      //MySQL is used as cache
      case "MySQL":
	$db_handle = mysql_connect( $db_host, $db_user, $db_pass );
	if (!$db_handle)
	{
          echo "<p><font color=\"red\"><b>ERROR: Cannot open connection to database server ($db_host)!</b></font></p>";
          exit;
        }

        if (! mysql_select_db( $db_name, $db_handle ))
	{
          echo "<p><font color=\"red\"><b>ERROR: Cannot select database '$db_name'!</b></font></p>";
          exit;
        }
	$SQL = "CREATE TABLE `$tablename` (`Key` VARCHAR( 255 ) NOT NULL".
	       " ,`Value` TEXT NOT NULL , PRIMARY KEY ( `Key` ) );";
        $db_tablename = create_table($db_handle, $db_name, $db_tablename, $SQL);//creates the table if necessary and returns the tablename

        if (!$db_tablename)
        {
          echo '<p><font color="red"><b>ERROR: Cannot create cache table!</b></font></p>';
          echo mysql_error( $db_handle );
          exit;
        }
        break;

      // unsupported DBMS
      default:
	    echo '<p><font color="red"><b>ERROR: specified DBMS is not yet supported!</b></font></p>';
        exit;
    }
  }
}

function close_cache()
{
  global $db_handle;
  global $DBMS;

  if ($db_handle !== False)
  {
    switch ($DBMS)
    {
      //DBA is used as cache
      case "DBA":
        dba_close($db_handle);
        $db_handle = False;
        break;

      //MySQL is used as cache
      case "MySQL":
		mysql_close($db_handle);
		$db_handle = False;
  	    break;
    }
  }
}

function make_db_key( $cathegory, $level )
{
  return $cathegory . "//" . $level;
}

function fetch_from_DB( $cathegory, $level )
{
  global $db_handle;
  global $DBMS;
  global $db_tablename;

  switch ($DBMS)
  {
    case "DBA":
      $data = dba_fetch( make_db_key( $cathegory, $level ), $db_handle );
      break;

    case "MySQL":
      $SQL = "SELECT * FROM `$db_tablename` WHERE `Key`='" . make_db_key( $cathegory, $level ) . "'";
      $res = mysql_query( $SQL, $db_handle );
      if (mysql_num_rows($res) != 1)
      	$data = False;
      else
        list(,$data) = mysql_fetch_row( $res );
      break;
  }

  if ( !$data )
    return False;
  else
  {
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
    return $ret;
  }
}

function level_cache_get( $cathegory, $level, $recache )
{
  global $db_handle;
  open_cache();
  $data = fetch_from_DB( $cathegory, $level );
  if ( $data == False || $recache == True )
  {
    $levelfile = sandbox_check( "data/levels/$cathegory/$level.pingus", "data/" );
    $leveldata = parse_level( $levelfile );
    $leveldata["md5sum"] = md5(implode("", file($levelfile)));

    $comments = parse_level_comments($cathegory, $level, "ASC");
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
  return $data;
}

function level_cache_del( $cathegory, $level )
{
  global $db_handle;
  global $DBMS;
  global $db_tablename;

  open_cache();

  switch ($DBMS)
  {
    case "DBA":
      $ret = dba_delete(make_db_key( $cathegory, $level ), $db_handle);
  	  break;

  	case "MySQL":
  	  $key = make_db_key( $cathegory, $level );
  	  $SQL = "DELETE * FROM $db_tablename WHERE LevelID='$key'";
  	  $ret = mysql_query( $SQL, $db_handle );
  	  break;
  }

  if ( !$ret )
  {
    echo '<p><font color="red"><b>ERROR: Cannot delete cache key!</b></font></p>';
    return False;
  }
  else
    return True;
}

function level_cache_save( $cathegory, $level, $arr )
{
  global $db_handle;
  global $db_tablename;
  global $DBMS;

  open_cache();
  $data = "";
  $key = make_db_key( $cathegory, $level );
  reset( $arr );

  while (list($k,$v) = each($arr))
    $data .= $k . "=" . $v . "\255";

  switch ($DBMS)
  {
    case "DBA":
	  $ret = dba_replace( make_db_key( $cathegory, $level ), $data, $db_handle );
      break;

    case "MySQL":
      $SQL = "SELECT * FROM `$db_tablename` WHERE `Key`='" . make_db_key( $cathegory, $level ) . "'";
      $ret = mysql_query( $SQL, $db_handle );
      if (mysql_num_rows( $ret ) == 1)
        $SQL = "UPDATE `$db_tablename` SET `Value`='" . str_replace( "'", "\'", $data ) . "' WHERE `Key`='$key'";
      else
        $SQL = "INSERT INTO $db_tablename (`Key`,`Value`) VALUES ('$key','" . str_replace( "'", "\'", $data ) . "')";
      $ret = mysql_query( $SQL, $db_handle );
      break;
  }

  if (!$ret)
  {
    echo '<p><font color="red"><b>ERROR: Cannot save level cache entry!</b></font></p>';
    return False;
  }
  else
    return True;
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

function parse_level_comments( $cathegory, $level, $order )
{
  $res = Array();
  $comment_dir = "comments/$cathegory/$level";
  if ( !is_dir( $comment_dir ))
    return $res;
  sandbox_check($comment_dir, "comments/");

  $dir = dir($comment_dir);
  $comments = Array();
  while ($file = $dir->read())
    if ($file != "." && $file != ".." && file_exists("$comment_dir/$file") && !is_dir("$comment_dir/$file"))
      $comments[] = "$comment_dir/$file";
  $dir->close();

  while( list($key, $filename) = each($comments))
  {
    $cmt = xml_search( $filename, Array(
      "author" => "pingus-level-comment/author",
      "email" => "pingus-level-comment/email",
      "date" => "pingus-level-comment/date",
      "time" => "pingus-level-comment/time",
      "difficulty" => "pingus-level-comment/difficulty",
      "rating" => "pingus-level-comment/rating",
      "comment" => "pingus-level-comment/comment",
      "levelmd5" => "pingus-level-comment/levelmd5"
    ));

    // We don't need tag names and attributes, strip them off:
    while( list($k,$v) = each($cmt))
      $cmt[$k] = $v["v"];

    $cmt["filename"] = $filename;

    // is the time set? (it is not in old comment files)
    if (isset($cmt["time"]))
      $res[$cmt["date"] . '-' . $cmt["time"]] = $cmt;
    else
      $res[$cmt["date"] . '-' . substr(md5(implode("", file($filename))),0,8)] = $cmt;
  }
  if ($order == "DESC")
    arsort($res);
  else
    asort($res);

  return $res;
}

function parse_level_demos( $cathegory, $level )
{
  $res = Array();
  $demo_dir = "comments/$cathegory/$level/demos";
  if ( !is_dir( $demo_dir ))
    return $res;
  sandbox_check($demo_dir, "comments/");

  $dir = dir($demo_dir);
  $demos = Array();
  while ($file = $dir->read())
    if ($file != "." && $file != ".." && file_exists("$demo_dir/$file")
        && !is_dir("$demo_dir/$file") && strpos( $file, ".demo" ))
      $demos[] = "$demo_dir/$file";
  $dir->close();

  while( list($key, $filename) = each($demos))
  {
    $cmt = xml_search( $filename, Array(
      "username" => "pingus-demo-metafile/username",
      "email" => "pingus-demo-metafile/email",
      "date" => "pingus-demo-metafile/date",
      "time" => "pingus-demo-metafile/time",
      "demofile" => "pingus-demo-metafile/demofile",
      "levelmd5" => "pingus-demo-metafile/levelmd5"
    ));

    // We don't need tag names and attributes, strip them off:
    while( list($k,$v) = each($cmt))
      $cmt[$k] = $v["v"];

    $cmt["filename"] = $filename;

    $res[$cmt["date"] . '-' . $cmt["time"]] = $cmt;
  }
  asort($res);

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
