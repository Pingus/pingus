<?

$mail_notify_enabled = True;

//$mail_from = "Pingus level comment tool <pingus-devel@nongnu.org>";
//$mail_replyto = "Pingus developer list <pingus-devel@nongnu.org>";
$mail_from = "Pingus level comment tool <elonen@iki.fi>";
$mail_replyto = "Pingus developer list <elonen@iki.fi>";
$mail_forwards = Array(
  "obsolete@example.com" => "new-one@example.com",
  "reluctant@example.com" => null
);

$mail_subject = "[Pingus levels] comments on @CATHEGORY/@LEVEL";
$mail_message = <<<EOMSG
@SENDER
has commented on your Pingus level at
http://elonen.iki.fi/pingus/?c=@CATHEGORY&l=@LEVEL

Rating: @RATING
Difficulty: @DIFFICULTY
Comments:

---------
@COMMENTS
---------

This is an automatically generated notification mail.
If you wish not to receive comments anymore, write a
short note to Pingus developer list pingus-devel@nongnu.org.
EOMSG;

?>