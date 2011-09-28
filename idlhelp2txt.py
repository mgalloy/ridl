#!/usr/bin/env python

from HTMLParser import HTMLParser
from optparse import OptionParser
import os


class IDLHelpParser(HTMLParser):
  
  def handle_charref(self, name):
    if (name == "160"):
      self.current_text = self.current_text + " "
      
  def handle_data(self, data):
    # compress whitespace
    stripped_data = " ".join(data.split())
    
    # exit if nothing left
    if (len(stripped_data) == 0):
      return
      
    # keep a single space at beginning or end if they were there to start with
    if (data[0] != stripped_data[0]):
      stripped_data = " " + stripped_data
    if (data[-1] != stripped_data[-1]):
      stripped_data = stripped_data + " "
    
    # append to current text
    self.current_text = self.current_text + stripped_data

  def handle_starttag(self, tag, attrs):    
    if (tag == "p"):
      self.current_text = ""
      
      # assume the paragraph is a normal paragraph
      self.type = "plain_para"

      # hide if it has specific attributes
      for a in attrs:
        if (a[0] == "class" and a[1] == "Code"):
          self.type = "code_para"
          self.last_was_code_para = True
          
        if (a[0] == "class" and a[1] == "MCWebHelpFramesetLink"):
          self.type = "hide_para"
        if (a[0] == "MadCap:conditions" and a[1] == "Reference Material.Online_Help_Only"):
          self.type = "hide_para"
        if (a[0] == "class" and a[1] == "HideSearchTerms"):
          self.type = "hide_para"                 
      
    if (tag == "h1" or tag == "h2" or tag == "h3"):
      self.current_text = ""
      self.type = "header"
  
    if (tag == "img"):
      self.current_text = "[image]"
      self.type = "image"
      
    if (tag == "br"):
      self.current_text = self.current_text + "\n"
      
  def handle_endtag(self, tag):
    if self.last_was_code_para:
      extra_line = "\n"
    else:
      extra_line = ""
      
    if (tag in self.para_tags):
      self.last_was_code_para = False
              
    if (tag == "p" and self.type == "plain_para"):
      if (len(self.current_text) > 0):        
        self.output = self.output + extra_line + self.current_text + "\n\n"
                
    if (tag == "p" and self.type == "code_para"):
      if (len(self.current_text) > 0):
        self.output = self.output + "  " + self.current_text + "\n"
        self.last_was_code_para = True
        
    if (tag == "h1" or tag == "h2" or tag == "h3"):
      if (tag == "h1"): char = "="
      if (tag == "h2"): char = "-"
      if (tag == "h3"): char = "~"
      if (tag == "h1"):
        self.output = self.output + extra_line + "".join([char for x in range(len(self.current_text))]) + "\n"
        extra_line = ""
      
      self.output = self.output + extra_line + self.current_text + "\n"
      self.output = self.output + "".join([char for x in range(len(self.current_text))]) + "\n"
    
    if (tag == "img"):
      self.output = self.output + extra_line + self.current_text + "\n\n"
  
  def reset(self):
    HTMLParser.reset(self)
    self.current_text = ""
    self.type = ""
    self.output = ""
    self.last_was_code_para = False
    
  def __init__(self):
    HTMLParser.__init__(self)
    self.para_tags = ["p", "img", "h1", "h2", "h3"]
    self.reset()


def idlhelp2txt_file(filename):
  f = open(filename, "r")
  lines = f.read()
  f.close()
  
  lines = lines.replace('<![CDATA[ ]]>', '')
  
  h = IDLHelpParser()
  h.feed(lines)
  return(h.output)
  

# idlhelp2txt.py helpdir outputdir
def main():
  parser = OptionParser()
  (options, args) = parser.parse_args()
  
  helpdir = os.path.normpath(args[0])
  outputdir = os.path.normpath(args[1])
  
  # make output directory if not already present
  if not os.path.isdir(outputdir):
    os.mkdir(outputdir)
  
  for dirpath, subdirs, files in os.walk(helpdir):
    print 'Processing directory: %s' % dirpath
    
    # find and create output path if it doesn't already exist
    outputpath = outputdir + dirpath[len(helpdir):]
    if not os.path.isdir(outputpath):
      os.mkdir(outputpath)
      
    # filter out anything not an .html file
    files = [f for f in files if f[-4:] == "html"]
    
    for basename in files:
      filename = os.path.join(dirpath, basename)
      outputname = os.path.join(outputpath, basename)
      (outputname, ext) = os.path.splitext(outputname)
      outputname = outputname + '.txt'
      output = idlhelp2txt_file(filename)
      f = open(outputname, "w")
      f.write(output)
      f.close()
      
  
if __name__ == "__main__":
  main()
