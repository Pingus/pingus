This folder is used to backup any extra information about Pingus that is available on the web.  Currently, it keeps a copy of the wiki located at:

http://www.cmhousing.net/pingus/wiki

So as to not lose this information, the following script must be run occassionally (whenever a decent amount of information has been updated at the wiki):

./download-wiki.sh

This script requires that you have the utility 'wget' installed on your system.  I know there has to be a better way of backing up the site, but this is the best one I could come up with on short notice.  It basically downloads everything on the site, then deletes the files that aren't needed.  After downloading everything locally, commit the entire www.cmhousing.net folder to the SVN repository.