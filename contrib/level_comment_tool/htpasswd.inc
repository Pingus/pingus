<?php

// .htpasswd file functions
// Copyright (C) 2004 Jarno Elonen <elonen@iki.fi>
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
//
// Usage
// =====
//   require_once('htpasswd.inc');
//   $pass_array = load_htpasswd();
//
//   if ( test_htpasswd( $pass_array,  $user, $pass ))
//       print "Access granted."
//
//   $pass_array[$new_user] = rand_salt_crypt($new_pass);
//   save_htpasswd($pass_array);

define("HTPASSWDFILE", ".htpasswd");

// Loads htpasswd file into an array of form
// Array( username => crypted_pass, ... )
function load_htpasswd()
{
  if ( !file_exists(HTPASSWDFILE))
      return Array();

  $res = Array();
  foreach(file(HTPASSWDFILE) as $l)
  {
    $array = explode(':',$l);
    $user = $array[0];
    $pass = chop($array[1]);
    $res[$user] = $pass;
  }
  return $res;
}

// Saves the array given by load_htpasswd
function save_htpasswd( $pass_array )
{
 ignore_user_abort(true);
  $fp = fopen(HTPASSWDFILE, "w+");
  if (flock($fp, LOCK_EX))
  {
    while( list($u,$p) = each($pass_array))
      fputs($fp, "$u:$p\n");
    flock($fp, LOCK_UN); // release the lock
  }
  else
  {
    print "<strong>ERROR! Could not save (lock) .htpasswd!</strong><br>";
  }
  fclose($fp);
  ignore_user_abort(false);
}

// Generates a htpasswd compatible crypted password string.
function rand_salt_crypt( $pass )
{
  // Randomize a 2-letter crypt() salt:
  $cset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789./";
  $salt = substr($cset, time() & 63, 1) .
          substr($cset, time()/64 & 63, 1);
  return crypt($pass, $salt);
}

// Returns true if the user exists and the password matches, false otherwise
function test_htpasswd( $pass_array, $user, $pass )
{
  if ( !isset($pass_array[$user]))
      return False;
  $crypted = $pass_array[$user];
  return crypt( $pass, substr($crypted,0,2) ) == $crypted;
}

?>