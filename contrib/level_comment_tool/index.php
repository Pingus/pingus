<?
//  Pingus Level Comment Tool
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
require_once("level-cache.inc");
require_once("mail-settings.inc");
require_once("htpasswd.inc");

// ==================================================================
// Admin login.
// ==================================================================

$htpasswd = load_htpasswd();
$is_admin = False;
if ( isset($_SERVER['PHP_AUTH_PW']))
  $is_admin = test_htpasswd( $htpasswd,  "admin", $_SERVER['PHP_AUTH_PW'] );

if ( isset($_GET["adminlogin"]))
{
  if (!isset($_SERVER['PHP_AUTH_USER']))
  {
    header('WWW-Authenticate: Basic realm="Pingus Comment Tool Admin"');
    header('HTTP/1.0 401 Unauthorized');
    echo 'Cancelled. Hit Back.';
    exit;
  }
  else
  {
    if ( !isset($_SERVER['PHP_AUTH_PW']) ||
      !test_htpasswd( $htpasswd,  "admin", $_SERVER['PHP_AUTH_PW'] ))
    {
      header('HTTP/1.0 401 Unauthorized');
      echo 'Wrong password. Hit Back.';
      exit;
    }
    else
      $is_admin = True;
  }
}

// ==================================================================
// Thumbnail magick with cookies etc.
// ==================================================================
if (isset($_GET["showthumbs"]))
{
  $show_thumbs = $_GET["showthumbs"];
  if (!isset($_COOKIE["showthumbs"]) || $_COOKIE["showthumbs"] != $show_thumbs )
    setCookie( "showthumbs", $_GET["showthumbs"], time()+3600*24 );
}
else if ( isset($_COOKIE["showthumbs"]) )
  $show_thumbs = $_COOKIE["showthumbs"];

// ==================================================================

?>
<html>
  <head>
    <title>Pingus level comment database</title>
    <link rel="stylesheet" type="text/css" href="default.css">
  </head>
  <body>

    <h1>Pingus level comment database</h1>
    <div class="mainbody">

<?

$preferred_order = Array( 'tutorial', 'playable', 'volcano', 'wip', 'test' );

// ==================================================================
// Read cathegory and level names from filesystem
// ==================================================================
$cathegories = Array();
$dir = dir("data/levels");
while ($file = $dir->read())
{
  if ($file != "." && $file != ".." && is_dir("data/levels/".$file))
  {
    $levels = Array();
    $dir2 = dir("data/levels/".$file);
    while ($file2 = $dir2->read())
      if (strpos($file2, ".pingus") !== FALSE)
        $levels[] = str_replace( ".pingus", "", $file2 );
    $dir2->close();
    natcasesort( $levels );

    $cath = Array();
    $cath["name"] = $file;
    $cath["levels"] = $levels;
    $key = array_search($cath["name"], $preferred_order);
    if ( $key === False )
      $key = max(count($preferred_order), count($cathegories)+1);
    $cathegories[$key] = $cath;
  }
}
$dir->close();
ksort( $cathegories );

// ==================================================================
// Level listing
// ==================================================================
if  ( !isset($_GET["l"]) || !isset($_GET["c"]) )
{
  if ( !isset($_GET["c"]) )
    $showCath = '';
  else
    $showCath = '&c=' . $_GET["c"];

  if ( $show_thumbs )
  	print "<a href='$PHP_SELF?showthumbs=0$showCath'>[hide thumbnails]</a></br>";

  else
    print "<a href='$PHP_SELF?showthumbs=1$showCath'>[show thumbnails]</a></br>";

  if ( !isset($_GET["c"]))
  {
    print "<p><strong>Pick a cathegory</strong>\n<ul>\n";
    while( list(,$c) = each( $cathegories ))
      print "<li><a href='$PHP_SELF?c=" . urlencode($c["name"]) . "'>" .
        htmlentities($c["name"]) . "</a></li>\n";
    print "</ul>\n";
  }
  else
  {
    print "<a href='$PHP_SELF'>[Back to cathegory list]</a></br>\n";
    print "<a href='file-assoc-help.html' target='helpwin'>(Help: how to make 'play' link work)</a></br>\n";
    while( list(,$c) = each( $cathegories ))
    {
      if ( $_GET["c"] == $c["name"] )
      {
        $cnt = count($c["levels"]);
        print "<h2 align='center'>" . htmlentities( $c["name"] ) . " ($cnt)" . "</h2>\n";

        print "<table style='margin-bottom: 1em;' width='100%' ".
          "border='0' cellspacing='0' cellpadding='4'><tr>\n";
        $i = $col_cnt = 0;
        while( list(,$l) = each( $c["levels"] ))
        {
          unset( $ldata );
          $ldata = level_cache_get( $c["name"], $l, False );
          $cnt = $ldata["totalcomments"];

          if ( ($i++) % 2 )
            $celcolor = "bgcolor='#B0B0B0'";
          else
            $celcolor = "bgcolor='#A0A0A0'";

          $jpg = htmlentities($c["name"]) . "/" . htmlentities($l) . ".jpg";
          print "<td valign='top' $celcolor width='50%'>\n";
          if ( $show_thumbs )//additional column only when thumbs are shown
	      {
            print "<table width='100%' border='0'>\n" .
             "<tr><td style='width:160px; height:120px;'>" .
             "<a href='$PHP_SELF?c=" . urlencode($c["name"]) . "&l=" . urlencode($l) . "'>".
             "<img src='http://pingus.seul.org/levels/thumb/$jpg' align='left' border='0'>".
             "</a></td><td>\n";
          }

          print "<small>".
          "<a href='$PHP_SELF?c=" .
          urlencode($c["name"]) . "&l=" . urlencode($l) . "'>".
          urlencode($l) . "</a>\n";

          if ( $cnt < 1 ) $cnt = "-";
          print " (<strong>$cnt</strong>)<br>";
          print "<em>&quot;" . htmlentities($ldata["name"]) . "&quot;</em><br>";
          if ( strlen($ldata["difficulty"]))
            print "Difficulty:" . htmlentities($ldata["difficulty"]) . "<br>";
          print "Pingus:" . intval($ldata["pingusmin"]) . "/" . intval($ldata["pingusmax"]) . "<br>";
          if ( intval($ldata["playable"]) == 0 )
            print "<b>Not playable</b><br>";
          if ( $cnt > 0 )
            print "Rating:" . str_repeat( "*", intval($ldata["avgrating"])) . "<br>";
          print " view <a href='http://pingus.seul.org/levels/33/$jpg' target='levelview'>third</a>".
            " / <a href='http://pingus.seul.org/levels/50/$jpg' target='levelview'>half</a>" .
            " / <a href='http://pingus.seul.org/levels/100/$jpg' target='levelview'>full</a><br>" .
            /*"play with <a href='data/levels/" . urlencode($c["name"]) . "/" . urlencode($l) . ".pingus'>Subversion</a>" .
            " / <a href='data/levels/" . urlencode($c["name"]) . "/" . urlencode($l) . ".xml'>0.6</a> version" .
            "<br>";*/
            "<a href='data/levels/" . urlencode($c["name"]) . "/" . urlencode($l) . ".pingus'>play level</a>" .
            " (requires Pingus 0.6.1 or later)".
            "</small>\n";
          if ( $show_thumbs )
            print "</td></tr></table>\n";
          print "</td>\n";


          if ($col_cnt++ == 1 )
          {
            print "</tr><tr>\n";
            $col_cnt = 0;
            $i++; // kludge to make coloring look like checkers.
          }
        }
        print "</tr></table>\n";
      }
    }
  }
}
// ==================================================================
// Info on certain level
// ==================================================================
else
{
  $c = $_GET["c"];
  $l = $_GET["l"];
  $curlevelmd5 = FALSE;

  $levelfile = sandbox_check( "data/levels/$c/$l.pingus", "data/" );
  if ( !is_file( $levelfile ))
  {
    print "<h2>Level '" . htmlentities($c) . " / " .
      htmlentities($l) . "' not found.</h2>\n";
    print "<p><a href='$PHP_SELF?c=" . urlencode($c) . "'>Back to level list</a></p>";
  }
  else
  {
    print "<h2>" . htmlentities($c) . " / " . htmlentities($l) . "</h2>\n";

    $curlevelmd5 = md5(implode("", file($levelfile)));
    $leveldata = level_cache_get( $c, $l, False );
    if ( $leveldata["md5sum"] !== $curlevelmd5 )
      $leveldata = level_cache_get( $c, $l, True );
    $leveldata["totalcomments"] = 0;
    $leveldata["avgrating"] = 0;
    $leveldata["md5sum"] = $curlevelmd5;

    // Level description
    $tbl = Array(
      "Name" => htmlentities($leveldata["name"]),
      "Description" => htmlentities($leveldata["description"]),
      "Comment" => htmlentities($leveldata["comment"]),
      "Author" => str_replace("@", "<b><small>PingusNoSpam</small></b>@",
        htmlentities($leveldata["author"])),
      );
    print( "<table border='1'>\n" );
    while( list($key,$val) = each( $tbl ))
      print("<tr><td><strong>" . htmlentities($key) . "</strong></td>\n" .
            "<td colspan='2'>" . $val . "</td></tr>\n");

    // Basic info about the level
    $tbl = Array(
      "Pingus #" => $leveldata["pingusmin"] ." / " . $leveldata["pingusmax"],
      "Time" => $leveldata["time"],
      "Difficulty" => $leveldata["difficulty"],
      "Playable" => $leveldata["playable"],
      );
    $i = 0;
    while( list($key,$val) = each( $tbl ))
    {
      print("<tr><td><strong>" . $key . "</strong></td>");
      print ("<td width='100%'>\n" . htmlentities($val) . "</td>");
      // Thumbnail
      if ( $i++ == 0)
      {
        $jpg = htmlentities($c) . "/" . htmlentities($l) . ".jpg";
        print ("<td rowspan='5' valign='top' align='center'>\n".
          "  <img src='http://pingus.seul.org/levels/thumb/$jpg' border='1'><br/>\n".
          "  see <a href='http://pingus.seul.org/levels/33/$jpg' target='levelview'>third</a> /".
          " <a href='http://pingus.seul.org/levels/50/$jpg' target='levelview'>half</a> / \n".
          "  <a href='http://pingus.seul.org/levels/100/$jpg' target='levelview'>full</a> size\n<br>".
          "<a href='data/levels/" . htmlentities($c) . "/" . htmlentities($l) . ".pingus'>play</a>\n" .
          "</td>\n" );
      }
    }

    print "</tr><tr><td><strong>Actions</strong></td><td>\n";

    // Number of actions
    reset( $leveldata );
    while( list($k,$v) = each($leveldata))
    {
      if ( substr($k,0,1)=="#" )
      {
        $a = substr($k,1);
        $ac = trim($v);
        if ( strlen($ac) == 0 )
          $ac = "<strong>?</strong>";
        else
          $ac = htmlentities($ac);
          print ("<img id='Pic$a' name='$a' alt='$a' title='$a' " .
                 "style='cursor: pointer' src='gfx/actions/$a.png' " .
                 "onclick='document.getElementById(\"CommentForm\").comment.value = " .
                 "document.getElementById(\"CommentForm\").comment.value + " .
                 "document.getElementById(\"Pic$a\").name'></a> x " .
                 $ac . " ");
      }
    }

    print( "</td></tr></table>\n" );
?>
    </p>
    <hr/>
<?
    // ==================================================================
    // Save added comment
    // ==================================================================
    if ( isset($_POST["addcomment"]))
    {
      if ( strpos($c, "..") !== False || strpos($c, "/") !== False ||
        strpos($l, "..") !== False || strpos($l, "/") !== False )
      {
        print "<strong>ERROR: Illegal level name.</strong>";
        exit;
      }

      if ( !is_dir("comments/$c/$l") &&
           (@mkdir("comments/$c", 0775) || True) &&
           !(@mkdir("comments/$c/$l", 0775)))
      {
        print ("<strong>ERROR: 'comments/$c/$l/' does not exist and ".
               "could not be created.</strong>\n");
        exit;
      }
      sandbox_check("comments/$c/$l", "comments/" );
      $str =
        '<' . '?xml version="1.0"  encoding="ISO-8859-1"?' . ">\n" .
        "<pingus-level-comment>\n".
        "  <author>" . xmlentities(decode_html(stripslashes($_POST["author"]))) . "</author>\n" .
        "  <email>" . xmlentities(decode_html(stripslashes($_POST["email"]))) . "</email>\n" .
        "  <level>" . xmlentities("$c/$l") . "</level>\n" .
        "  <levelmd5>" . xmlentities(stripslashes($_POST["levelmd5"])) . "</levelmd5>\n" .
        "  <date>" . xmlentities(date("Y-m-d")) . "</date>\n" .
        "  <time>" . xmlentities(date("H:i:s")) . "</time>\n" .        
        "  <difficulty>" . xmlentities(stripslashes($_POST["difficulty"])) . "</difficulty>\n" .
        "  <rating>" . xmlentities(stripslashes($_POST["rating"])) . "</rating>\n" .
        "  <comment>" . xmlentities(decode_html(stripslashes($_POST["comment"]))) . "</comment>\n" .
        "</pingus-level-comment>\n";

      $filename = "comments/$c/$l/" . substr(md5($str),0,8) . ".pingus";
      if ( !file_exists($filename) || is_writable($filename))
      {
        if (!$fp = fopen($filename, 'w'))
        {
          print "<strong>ERROR: Cannot open file ($filename)</strong>\n";
          exit;
        }
        if (!fwrite($fp, $str))
        {
          print "<strong>ERROR: Cannot write to file ($filename)</strong>\n";
          exit;
        }
        fclose($fp);
        chmod($filename, 0775);

        // Send email-notification
        if ( $mail_notify_enabled )
        {
          $mailto = trim($leveldata["author"]);
          if ( isset($mail_forwards[$mailto]))
            $mailto = trim($mail_forwards[$mailto]);
          if ( $mailto !== null && strlen($mailto) > 0 && strpos($mailto,"@") > 0 )
          {
            $repl = Array(
              "@SENDER" => decode_html(stripslashes($_POST["author"])) .
                " <" . decode_html(stripslashes($_POST["email"])) . ">",
              "@CATHEGORY" => $c,
              "@LEVEL" => $l,
              "@TO" => $mailto,
              "@RATING" => str_repeat( "*", intval(stripslashes($_POST["rating"]))),
              "@DIFFICULTY" => stripslashes($_POST["difficulty"]),
              "@COMMENTS" => stripslashes(decode_html($_POST["comment"]))
            );

            mail(
              $mailto,
              str_replace(array_keys($repl), array_values($repl), $mail_subject),
              str_replace(array_keys($repl), array_values($repl), $mail_message),
              "From: $mail_from\r\n"
                ."Reply-To: $mail_replyto\r\n"
                ."X-Mailer: PHP/" . phpversion());
          }
        }
      }
      else
        print "<strong>ERROR: File $filename is not writable</strong>\n";
    }
    // ==================================================================
    // Delete comment
    // ==================================================================
    if ( $is_admin && isset($_GET["delcomment"]))
    {
      sandbox_check($_GET["delcomment"], "comments/" );
      unlink($_GET["delcomment"]);
      // Also try to remove the directories but don't mind if it fails:
      @rmdir("comments/$c/$l");
      @rmdir("comments/$c");
      print "<p><strong>Deleted '" . htmlentities($_GET["delcomment"]) . "'</strong></p>";
    }

	// ==================================================================
    // Save uploaded Demofile
    // ==================================================================
    if ( isset($_POST["adddemo"]))
    {
      //create directory if necessary
      if ( !is_dir("comments/$c/$l/demos") &&
           (@mkdir("comments/$c", 0775) || True) &&
           (@mkdir("comments/$c/$l", 0775) || True) &&
           !(@mkdir("comments/$c/$l/demos", 0775)))
      {
        print ("<strong>ERROR: 'comments/$c/$l/demos' does not exist and ".
               "could not be created.</strong>\n");
        exit;
      }
      
      $uploaddir = sandbox_check( "comments/$c/$l/demos", "comments/" );
      
      if(!strpos( strtolower( $_FILES["demofile"]["name"] ), ".xml" ))
        print("<strong> Please upload only .xml files </strong><hr/>");
      else
      {
        if (move_uploaded_file($_FILES['demofile']['tmp_name'], $uploaddir. '/' . $_FILES['demofile']['name']))
        {
          print("<strong>File uploaded successfully</strong><hr/>");
          $str = '<' . '?xml version="1.0"  encoding="ISO-8859-1"?' . ">\n" .
                "<pingus-demo-metafile>\n".
                "  <username>" . xmlentities(decode_html(stripslashes($_POST["username"]))) . "</username>\n" .
                "  <email>" . xmlentities(decode_html(stripslashes($_POST["email"]))) . "</email>\n" .
                "  <level>" . xmlentities("$c/$l") . "</level>\n" .
                "  <levelmd5>" . xmlentities(stripslashes($_POST["levelmd5"])) . "</levelmd5>\n" .
                "  <date>" . xmlentities(date("Y-m-d")) . "</date>\n" .
                "  <time>" . xmlentities(date("H:i:s")) . "</time>\n" .        
                "  <demofile>" . xmlentities( $_FILES['demofile']['name'] ) . "</demofile>\n" .
                "</pingus-demo-metafile>\n";
            
          $filename = "comments/$c/$l/demos/" . substr(md5($str),0,8) . ".demo";
          if ( !file_exists($filename) || is_writable($filename))
          {
            if (!$fp = fopen($filename, 'w'))
            {
              print "<strong>ERROR: Cannot open file ($filename)</strong>\n";
              exit;
            }
            if (!fwrite($fp, $str))
            {
              print "<strong>ERROR: Cannot write to file ($filename)</strong>\n";
              exit;
            }
            fclose($fp);
            chmod($filename, 0775);
     	  }
     	}  
        else
        {
          print("<strong>error:");
          print_r($_FILES);
          print("</strong><hr/>");
        }  
      }
    }
    
    // ==================================================================
    // List user comments and demo files
    // ==================================================================
    if (isset( $_GET["o"] )) //determine order of comments
    {
      $comments = parse_level_comments($c, $l, $_GET["o"]);
      if ($_GET["o"] == "ASC")
        $OrderLink = " (<a href='$PHP_SELF?c=$c&l=$l&o=DESC'>revert order</a>)";
      else
        $OrderLink = " (<a href='$PHP_SELF?c=$c&l=$l&o=ASC'>revert order</a>)"; 
    }    
    else  
    {
      $comments = parse_level_comments($c, $l, "ASC");
      $OrderLink = " (<a href='$PHP_SELF?c=$c&l=$l&o=DESC'>revert order</a>)";
    }  
    $showComments = False;
    $showDemos = False; 
    if ( count($comments) > 0 )
    {
      $showComments = True;
      while( list(,$cmt) = each($comments))
      {
        $leveldata["totalcomments"]++;

        $rating = intval($cmt["rating"]);
        $leveldata["avgrating"] += $rating;

        if ( $is_admin )
          $del_link = " <a href='$PHP_SELF?c=" . htmlentities($c) .
            "&l=" . htmlentities($l) . "&delcomment=" . htmlentities($cmt["filename"]) . "'>[del]</a>\n";
        $str =
          "<p class='message'><strong>From:</strong> " . htmlentities($cmt["author"]) .
          " &lt;" . str_replace("@", "<b><small>PingusNoSpam</small></b>@",
            htmlentities($cmt["email"])) .
            "&gt;, <strong>Date: </strong> " . htmlentities($cmt["date"]) . 
          ", <strong>Time: </strong> " . htmlentities($cmt["time"]) . "<br/>\n" .
          "<b>Difficulty: </b>" . htmlentities($cmt["difficulty"]) . "\n" .
          ", <b>Rating: </b>" . str_repeat( "*", intval($rating)) . $del_link . "<br/>\n" .
          "<cite>\n" .
            str_replace( "\n", "<br/>\n", htmlentities($cmt["comment"])) .
          "</cite></p>\n";

        if (strtolower($cmt["levelmd5"]) == strtolower($curlevelmd5))
          $new_comments_text .= $str;
        else
          $old_comments_text .= $str;
      }
    }  
     
    $demos = parse_level_demos( $c, $l );
    if ( count($demos) > 0 )
    {
      $showDemos = True;

      while( list(,$cmt) = each($demos))
      {
        $leveldata["totaldemos"]++;
        
        $str =
          "<p class='message'><strong>From:</strong> " . htmlentities($cmt["username"]) .
          " &lt;" . str_replace("@", "<b><small>PingusNoSpam</small></b>@", htmlentities($cmt["email"])) .
          "&gt;<br><strong>Date: </strong> " . htmlentities($cmt["date"]) . 
          ", <strong>Time: </strong> " . htmlentities($cmt["time"]) . "<br/>\n" .
          "<b>Demofile: </b><a href='comments/$c/$l/demos/" . $cmt["demofile"] . "'>" .
          $cmt["demofile"] . "</a><br/>\n" .
          "</p>\n";
		  
        if (strtolower($cmt["levelmd5"]) == strtolower($curlevelmd5))
          $new_demos_text .= $str;
        else
          $old_demos_text .= $str;
      }
    } 
    
    print("<table width='100%' cellpadding='10'><tr><td width='60%' valign='top'>");
    if ($showComments)
      print ("<table cellpadding='5'><tr><td valign='bottom'><h2>Comments</h2></td>" .
             "<td valign='bottom'><strong>$OrderLink</strong></td></tr></table>\n" . $new_comments_text .
             "<h3>Comments for older version(s)</h3>\n" . $old_comments_text);
    else
     print ("<em>No comments for this level yet!</em>");
     
    print("</td><td valign='top'>");    
    if ($showDemos)           
      print ("<table cellpadding='5'><tr><td valign='bottom'><h2>Demo Files</h2></td></tr></table>\n" .
             $new_demos_text . "<h3>Demos for older version(s)</h3>\n" . $old_demos_text);
	else
      print ("<em>No Demos for this level yet!</em>");
    
    print ("</td></tr></table>");

    // Calc avg rating
    if ( $leveldata["totalcomments"] > 0 )
      $leveldata["avgrating"] = round($leveldata["avgrating"] / $leveldata["totalcomments"]);

    // Save cache entry
    level_cache_save( $c, $l, $leveldata );

    // Allow overriding level md5 sum in GET parameters
    $levelmd5 = $curlevelmd5;
    if ( isset( $_GET["levelmd5"] ))
      $levelmd5 = $_GET["levelmd5"];
?>
    <hr/>
    <table width="100%">
      <tr>
        <td width="60%" valign="top">
          <p><em><strong>Add a comment:</strong></em></p>
          <form id="CommentForm" action="<? echo $PHP_SELF . "?c=" . urlencode($c) . "&l=" . urlencode($l); ?>" method="POST">
            <input type="hidden" name="addcomment" value="1">
            <input type="hidden" name="c" value="<? echo urlencode("$c"); ?>">
            <input type="hidden" name="l" value="<? echo urlencode("$l"); ?>">
            <input type="hidden" name="levelmd5" value="<? echo urlencode($levelmd5); ?>">
            <table>
              <tr>
                <td>Your name</td>
                <td><input type="text" name="author"></td>
              </tr>
              <tr>
                <td>Your email</td>
                <td><input type="text" name="email"></td>
              </tr>
              <tr>
                <td>Difficulty</td>
                <td>
                  <select name="difficulty">
                    <option value="easy">easy</option>
                    <option value="normal">normal</option>
                    <option value="hard">hard</option>
                    <option value="very hard">very hard</option>
                    <option value="unplayable">unplayable</option>
                    <option value="unfinished" selected>unfinished</option>
                  </select>
                </td>
              </tr>
              <tr>
                <td>Rating</td>
                <td>
                  <select name="rating">
                    <option value="1">1 bad</option>
                    <option value="2">2 well...</option>
                    <option value="3" selected>3 ok</option>
                    <option value="4">4 good</option>
                    <option value="5">5 very good</option>
                  </select>
                </td>
              </tr>
              <tr>
                <td>Comments</td>
                <td><textarea rows="8" cols="40" name="comment"></textarea></td>
              </tr>
              <tr>
                <td colspan="2"><input type="submit"/></td>
              </tr>
            </table>
          </form>
        </td>
        <td width="50%" valign="top">
          <p><em><strong>Upload a demofile:</strong></em></p>
          <form enctype="multipart/form-data"  name="UploadDemo" action="<? echo $PHP_SELF . "?c=" . urlencode($c) . "&l=" . urlencode($l); ?>" method="POST">
            <input type="hidden" name="adddemo" value="1">
            <input type="hidden" name="c" value="<? echo urlencode("$c"); ?>">
            <input type="hidden" name="l" value="<? echo urlencode("$l"); ?>">
            <input type="hidden" name="levelmd5" value="<? echo urlencode($levelmd5); ?>">
            <table>
              <tr>
                <td>Your name</td>
                <td><input type="text" name="username"></td>
              </tr>
              <tr>
                <td>Your email</td>
                <td><input type="text" name="email"></td>
              </tr>
              <tr>
                <td>Demofile</td>
                <td>
                  <input type="file" name="demofile">
                </td>
              </tr>
              <tr>
                <td colspan="2">
                  <input type="submit" value="Upload file">
              	</td>
              </tr>
            </table>
          </form>
        </td>
      </tr>
    </table>      
          
<?
    print "<p><a href='$PHP_SELF?c=" . urlencode($c) . "'>Back to level list</a></p>";
    if ( !$is_admin )
      print "<div align='right'><a href='$PHP_SELF?adminlogin=1'><small>admin login</small></a></div>";
  }
}

close_cache();

?>
    </div>
    <div>
      <small>Pingus level comment tool &copy; 2003, 2004 by Jarno Elonen,
      Licensed under the Modified BSD License
      </small>
    </div>
  </body>
</html>

<?
// ==================================================================
// Some utilities
// ==================================================================
function xmlentities($text, $op=1)
{
  if ( $op == 1 )
  {
    $tbl = get_html_translation_table( HTML_ENTITIES, ENT_QUOTES );
    $tbl = array_flip( $tbl );
    $text = strtr( $text, $tbl );
  }

  $text = htmlspecialchars( $text, ENT_QUOTES );
  return $text;
}

function decode_html($string)
{
  $string = strtr($string, array_flip(get_html_translation_table( HTML_ENTITIES, ENT_QUOTES )));
  $string = preg_replace("/&#([0-9]+);/me", "chr('\\1')", $string);
  return $string;
}
?>
