<?php

// functions concering user signup and login
// this file needs a working MySQL database with the correct values
// set in level-cache.inc!!!
//
// Copyright (C) 2004 Björn Fischer <bjoern@bjoernfischer.de>
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// * The name of the author may not be used to endorse or promote products derived
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



// ==================================================================
// opens database connection
// ==================================================================
function open_db()
{
  global $db_conn;
  global $db_name;
  global $db_host;
  global $db_user;
  global $db_pass;
  
  $db_conn = mysql_connect( $db_host, $db_user, $db_pass );
  if (!$db_conn)
  {
    echo "<p><font color=\"red\"><b>ERROR: Cannot open connection to database server ($db_host)!</b></font></p>";
    exit;
  }

  if (! mysql_select_db( $db_name, $db_conn ))
  {
    echo "<p><font color=\"red\"><b>ERROR: Cannot select database '$db_name'!</b></font></p>";
    exit;
  }
}

// ==================================================================
// closes database connection
// ==================================================================
function close_db()
{
  global $db_conn;
  
  if ($db_conn !== False)
  {
    mysql_close($db_conn);
    $db_conn = False;
  }
}

// ==================================================================
// saves the dataset of a user
// ==================================================================
function save_user_data( $name, $email, $com_mailer, $update )
{
  global $db_conn;
  global $db_usertablename;
  global $db_name;
  
  if ($com_mailer == "on")
    $com_mailer = 1;
  else
    $com_mailer = 0;
  
  open_db();
  $SQL = "CREATE TABLE `$db_usertablename` (`ID` INT NOT NULL AUTO_INCREMENT ,`Name` VARCHAR( 100 ) NOT NULL ,".
         "`eMail` VARCHAR( 100 ) NOT NULL ,`commentmailer` TINYINT( 1 ) NOT NULL ,PRIMARY KEY ( `ID` ) ,".
         "UNIQUE (`eMail`));";
  $db_usertablename = create_table( $db_conn, $db_name, $db_usertablename, $SQL );
  
  if ($update)
    $SQL = "UPDATE $db_usertablename SET `Name`='$name',`commentmailer`=$com_mailer".
           " WHERE `eMail`='$email'";
  else  
    $SQL = "INSERT INTO $db_usertablename (`Name`,`eMail`,`commentmailer`)".
         " VALUES ('$name','$email',$com_mailer)";
         
  if (!mysql_query( $SQL, $db_conn ))
  {
    echo '<p><font color="red"><b>ERROR: Cannot save user data!</b></font></p>';
    close_db();
    return False;
  }
  else
  {
    close_db();
    return True;
  }
}

// ==================================================================
// loads user data from database
// ==================================================================
function get_user_data( $u_email )
{
  global $db_conn;
  global $db_usertablename;
  $ret = Array();
  
  open_db();
  
  $SQL = "SELECT * FROM $db_usertablename WHERE eMail='".strtolower($u_email)."'";
  $succ = mysql_query( $SQL, $db_conn );
  if (!$succ)
  {
    echo "<p><font color='red'><b>ERROR: Cannot find data for user $u_email!</b></font></p>";
    exit;
  }
  
  list( ,$name,$email,$com_mailer ) = mysql_fetch_row( $succ );
  $ret["name"] = htmlspecialchars($name);
  $ret["email"] = htmlspecialchars($email);
  $ret["com_mailer"] = htmlspecialchars($com_mailer);
  
  close_db();
  
  return $ret;
}

// ==================================================================
// Detetes users data from the table
// ==================================================================
function del_user_data( $u_email )
{
  global $db_conn;
  global $db_usertablename;
  
  open_db();
  $SQL = "DELETE FROM $db_usertablename WHERE eMail='".strtolower($u_email)."'";
  $succ = mysql_query( $SQL, $db_conn );  
  close_db();
  
  if ($succ)
    return True;
  else
    return False;
}

// ==================================================================
// Show a signup form
// ==================================================================
function show_signup_form()
{
  $params = "?" . get_params();
    
  echo '<script language = "JavaScript">'."\n".
       '<!--'."\n".
       'function TestError()'."\n".
       '{'."\n".
       '  var f = document.Signup;'."\n".
       '  var errors = "";'."\n".
       '  var i;'."\n".
       '  var IsError = false;'."\n\n".
       '  if ((f.elements["username"].value == "") || (f.elements["username"].value == null))'."\n".
       '  {'."\n".
       '    errors += "Please enter your Name!\n";'."\n".
       '    IsError = true;'."\n".
       '  } '."\n\n".
       '  if ((f.elements["email"].value == "") || (f.elements["email"].value == null))'."\n".
       '  {'."\n".
       '    errors += "Please enter your email adress!\n";'."\n".
       '    IsError = true;'."\n".
       '  } '."\n\n".
       '  if ((f.elements["pass"].value == "") || (f.elements["pass"].value == null))'."\n".
       '  {'."\n".
       '    errors += "Please choose a password!\n";'."\n".
       '    IsError = true;'."\n".
       '  } '."\n\n".
       '  if ((f.elements["pass2"].value == "") || (f.elements["pass2"].value == null))'."\n".
       '  {'."\n".
       '    errors += "Please retype your password!\n";'."\n".
       '    IsError = true;'."\n".
       '  }'."\n\n".
       '  if (f.elements["pass"].value != f.elements["pass2"].value)'."\n".
       '  {'."\n".
       '    errors += "The passwords do not match, please retype them!\n";'."\n".
       '    IsError = true;'."\n".
       '  }'."\n\n".
       '  if ((f.elements["email"].value.indexOf("@") == -1) || (f.elements["email"].value.length < 3))'."\n".
       '  {'."\n".
       '    errors += "Please enter a valid email adress!\n";'."\n".
       '    IsError = true;'."\n".
       '  }  '."\n\n".
       '  if (IsError==true)'."\n".
       '  {'."\n".
       '    alert(errors);'."\n".
       '    return false;'."\n".
       '  } '."\n".
       '  return true;'."\n".
       '}'."\n".
       '--></script>'."\n\n";
    
  echo "<H2>Pingus level comment database - user signup</H2>".
       "Please keep the following in mind:".
       "<UL><LI>enter your real name and a valid eMail adress</LI>".
       "<LI>please remember your password since there is no possibility to mail it to you yet</LI>".
       "<LI>after signup your login name is the entered mail adress<LI>".
       "</UL>By activating the checkbox you can".
       " activate the comment mailer, that means you will receive comments about your own levels".
       " by eMail (this is only important for level designers).<br><br>\n";
  
  echo '<form enctype="multipart/form-data"  name="Signup" action="'.$PHP_SELF.$params.'" method="POST" onsubmit="return TestError()">'."\n".
       '<input type="hidden" name="usersignedup" value="1">'."\n".
       '<table>'."\n".
       ' <tr>'."\n".
       '  <td>Your name</td>'."\n".
       '  <td><input type="text" name="username"></td>'."\n".
       ' </tr>'."\n".
       ' <tr>'."\n".
       '  <td>Your email</td>'."\n".
       '  <td><input type="text" name="email"></td>'."\n".
       ' </tr>'."\n".
       ' <tr>'."\n".
       '  <td>Comment mailer</td>'."\n".
       '  <td><input type="checkbox" name="commentmailer"></td>'."\n".
       ' </tr>'."\n".
       ' <tr>'."\n".
       '  <td>Pick a password</td>'."\n".
       '  <td><input type="password" name="pass"></td>'."\n".
       ' </tr>'."\n".
       ' <tr>'."\n".
       '  <td>Retype password</td>'."\n".
       '  <td><input type="password" name="pass2"></td>'."\n".
       ' </tr>'."\n".
       ' <tr>'."\n".
       '  <td colspan="2"><input type="submit" value="signup now"></td>'."\n".
       ' </tr>'."\n".
       '</table>'."\n".
       '</form>'."\n";
}

// ==================================================================
// Save singed up user data
// ==================================================================
function signup_user()
{
  global $DBMS;
  global $db_usertablename;
  global $db_conn;
  global $db_name;
  
  if (!isset( $_POST["usersignedup"] ))
    exit;
    
  if ($DBMS != "MySQL")
  {
    echo "<p><font color='red'><b>ERROR: MySQL support necessary. Signup not possible!</b></font></p>";
    exit;
  }
  
  $u_name = $_POST["username"];
  $u_email = strtolower($_POST["email"]);
  $u_com_mailer = $_POST["commentmailer"];
  $u_pass = $_POST["pass"];
  
  // look if user already exists
  $pass_arr = load_htpasswd();
  while (list($u, $p) = each($pass_arr))
  {
    if ($u_email == $u)
    {
      echo '<p><font color="red"><b>ERROR: This mailadress already has a user account!</b></font></p>';
      return False;
    }  
  }
  
  if (save_user_data( $u_name, $u_email, $u_com_mailer, False ))
  {
    $pass_arr[$u_email] = rand_salt_crypt($u_pass);
    save_htpasswd( $pass_arr );
    echo '<p><font color="green"><b>Your data has been saved. You can now login via the link on the right.<br>'.
         'Please remember that your eMail adress is used as login name<br></b></font></p>';
  }  
}

// ==================================================================
// Show list of users (with delete link for admin)
// ==================================================================
function show_user_list( $is_admin )
{
  $params = get_params();
  
  echo "<H2>Pingus level comment tool - user list</H2>\n";
  echo "<table border='1' cellpadding='10'>\n";
  echo "  <tr>\n";
  echo "    <th>\n";
  echo "      Name\n";
  echo "    </th>\n";
  echo "    <th>\n";
  echo "      eMail\n";
  echo "    </th>\n";
  echo "    <th>\n";
  echo "      Comment mailer\n";
  echo "    </th>\n";
  if ( $is_admin )
  {
    echo "    <th>\n";
    echo "      Delete\n";
    echo "    </th>\n";
  }
  echo "  </tr>\n";
  
  $htpasswd = load_htpasswd();
  while (list($u,$p) = each($htpasswd))
  {
    if ( $u != "admin" )
    {
      $u_data = get_user_data( $u );
      echo "  <tr>\n";
      echo "    <td>\n";
      echo "      ".htmlentities($u_data["name"])."\n";
      echo "    </td>\n";
      echo "    <td>\n";
      echo "      ".$u_data["email"]."\n";
      echo "    </td>\n";
      echo "    <td align='center'>\n";
      echo ($u_data["com_mailer"]==1)?"yes":"no";
      echo "\n";
      echo "    </td>\n";
      if ( $is_admin )
      {
        echo "    <td>\n";  
?>      
        <form enctype="multipart/form-data"  name="DeleteUser" 
              action="<? echo "$PHP_SELF?$params"; ?>" method="POST" 
              onsubmit="return confirm('Do you really want to delete user <? echo $u_data["email"]; ?>?')">
          <input type="hidden" name="deluser" value="1">
          <input type="hidden" name="user_email" value="<? echo $u_data["email"]; ?>">
          <input type="submit" value="delete">
        </form>   
        
<?
        echo "    </td>\n";
      }  
      echo "  </tr>\n";
    }  
  } //end while
  echo "</table>\n";
}

// ==================================================================
// Delete userdata from database
// ==================================================================
function delete_user( $u_email )
{
  $htpasswd = load_htpasswd();
  $new_htpasswd = array_flip( $htpasswd );
  
  $index = array_search( strtolower($u_email), $new_htpasswd );
  if (!$index)
  { 
    echo "<p><font color='red'><b>ERROR: User $u_email not found in .htpasswd!</b></font></p>";
  }
  else
  {
    $new_htpasswd = array_diff( $new_htpasswd, array( $index => strtolower($u_email)) ); 
    if (!del_user_data( $u_email ))
      echo "<p><font color='red'><b>ERROR: User $u_email not found in database!</b></font></p>";
    else  
    {
      $htpasswd = array_flip( $new_htpasswd );
      save_htpasswd( $htpasswd );
      echo "<p><font color='green'><b>User deleted successfully!</b></font></p>";
    }  
  }
}

// ==================================================================
// Show form to change user's data
// ==================================================================
function show_change_form( $u_email )
{
    $params = "?" . get_params();
    $user_data = get_user_data( $u_email );
    
    echo '<script language = "JavaScript">'."\n".
         '<!--'."\n".
         'function TestError()'."\n".
         '{'."\n".
         '  var f = document.Changeuser;'."\n".
         '  var errors = "";'."\n".
         '  var i;'."\n".
         '  var IsError = false;'."\n\n".
         '  if ((f.elements["username"].value == "") || (f.elements["username"].value == null))'."\n".
         '  {'."\n".
         '    errors += "Please enter your Name!\n";'."\n".
         '    IsError = true;'."\n".
         '  } '."\n\n".
         '  if (f.elements["pass"].value != f.elements["pass2"].value)'."\n".
         '  {'."\n".
         '    errors += "The passwords do not match, please retype them!\n";'."\n".
         '    IsError = true;'."\n".
         '  }'."\n\n".
         '  if ((f.elements["email"].value.indexOf("@") == -1) || (f.elements["email"].value.length < 3))'."\n".
         '  {'."\n".
         '    errors += "Please enter a valid email adress!\n";'."\n".
         '    IsError = true;'."\n".
         '  }  '."\n\n".   
         '  if (IsError==true)'."\n".
         '  {'."\n".
         '    alert(errors);'."\n".
         '    return false;'."\n".
         '  } '."\n".
         '  return true;'."\n".
         '}'."\n".
         '--></script>'."\n\n";
      
    echo "<H2>Change data for user ".$user_data["name"]."</H2>".
         "Please keep the following in mind:".
         "<UL><LI>enter your real name and a valid eMail adress</LI>".
         "<LI>please remember your password since there is no possibility to mail it to you yet</LI>".
         "<LI>always log in using your email adress as username<LI>".
         "</UL>By activating the checkbox you can".
         " activate the comment mailer, that means you will receive comments about your own levels".
         " by eMail (this is only important for level designers).<br><br>\n";
    
    echo '<form enctype="multipart/form-data"  name="Changeuser" action="'.$PHP_SELF.$params.'" method="POST" onsubmit="return TestError()">'."\n".
         '<input type="hidden" name="savechangeddata" value="1">'."\n".
         '<input type="hidden" name="email" value="'.$user_data["email"].'">'."\n".
         '<table>'."\n".
         ' <tr>'."\n".
         '  <td>Your name</td>'."\n".
         '  <td><input type="text" name="username" value="'.$user_data["name"].'"></td>'."\n".
         ' </tr>'."\n".
         ' <tr>'."\n".
         '  <td>Comment mailer</td>'."\n";
    if ( $user_data["com_mailer"] == 1 )
      echo '  <td><input type="checkbox" name="commentmailer" checked></td>'."\n";
    else  
      echo '  <td><input type="checkbox" name="commentmailer"></td>'."\n";
    
    echo ' </tr>'."\n".
         ' <tr>'."\n".
         '  <td>New password</td>'."\n".
         '  <td><input type="password" name="pass"></td>'."\n".
         ' </tr>'."\n".
         ' <tr>'."\n".
         '  <td>Retype password</td>'."\n".
         '  <td><input type="password" name="pass2"></td>'."\n".
         ' </tr>'."\n".
         ' <tr>'."\n".
         '  <td colspan="2"><input type="submit" value="update my data"></td>'."\n".
         ' </tr>'."\n".
         '</table>'."\n".
         '</form>'."\n".
         '<br>'."\n";
    
    echo '<form enctype="multipart/form-data"  name="Deluser" action="'.$PHP_SELF.$params.'" method="POST">'."\n".
         '<input type="hidden" name="deleteuser" value="1">'."\n".
         '<input type="hidden" name="email" value="'.$user_data["email"].'">'."\n".
         '<table>'."\n".
         ' <tr>'."\n".
	 "  <td><input style=\"background-color:#FF0000\" type=\"submit\" value=\"DELETE ACCOUNT!!!\"".
	        " onclick=\"return confirm('Do you really want to delete your user account?\\nThis will ".
	        "not be reverseable!');\"></td>"."\n".
         ' </tr>'."\n".
         '</table>'."\n".
         '</form>'."\n";
}

// ==================================================================
// Save changed user data
// ==================================================================
function change_user_data()
{
  global $DBMS;
  global $db_usertablename;
  global $db_conn;
  global $db_name;

  if (!isset( $_POST["savechangeddata"] ) && !isset( $_POST["deleteuser"] ))
    exit;
    
  if ($DBMS != "MySQL")
  {
    echo "<p><font color='red'><b>ERROR: MySQL support necessary. Signup not possible!</b></font></p>";
    exit;
  }
  
  if (isset( $_POST["savechangeddata"] ))
  {
    $u_name = $_POST["username"];
    $u_email = strtolower($_POST["email"]);
    $u_com_mailer = $_POST["commentmailer"];
    $u_pass = $_POST["pass"]; 
  
    $pass_arr = load_htpasswd();
    $user_found = False;
    while (list($u, $p) = each($pass_arr))
    {
      if ($u_email == $u)
      {
        $user_found = True;
      }  
    }
  
    if (!$user_found)
    {
      echo "<p><font color='red'><b>ERROR: User not found in .htpasswd!</b></font></p>";
      exit;
    }  
  
    if (save_user_data( $u_name, $u_email, $u_com_mailer, True))
    {
      if (strlen($u_pass) > 0)
      {
        $pass_arr[$u_email] = rand_salt_crypt($u_pass);
        save_htpasswd( $pass_arr );
      }  
      echo '<p><font color="green"><b>Your data has been updated.<br></b></font></p>';
    }  
  }
  else
    delete_user( strtolower($_POST["email"]) );  
}

?>