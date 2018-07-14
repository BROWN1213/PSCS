#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Mon Jul  9 22:14:01 2018

"""


#import geocoding_for_kml
import csv
import xml.dom.minidom
import sys


def extractCoord(row):
  # This extracts an address from a row and returns it as a string. This requires knowing
  # ahead of time what the columns are that hold the address information.
  return '%s,%s,%s' % (row['longitude'], row['latitude'], row['altitude'])

def createPlacemark(kmlDoc, row, order):
  # This creates a  element for a row of data.
  # A row is a dict.
  placemarkElement = kmlDoc.createElement('Placemark')
  
  #timeStampElement = kmlDoc.createElement('TimeStamp')
  #placemarkElement.appendChild(timeStampElement)
  
  styleUrlElement = kmlDoc.createElement('styleUrl')
  styleUrlElement.appendChild(kmlDoc.createTextNode('#hiker-icon'))
  placemarkElement.appendChild(styleUrlElement)
  
  pointElement = kmlDoc.createElement('Point')
  placemarkElement.appendChild(pointElement)
  
  coordinates = extractCoord(row)
  coorElement = kmlDoc.createElement('coordinates')
  coorElement.appendChild(kmlDoc.createTextNode(coordinates))
  pointElement.appendChild(coorElement)
  return placemarkElement

def createKML(csvReader, fileName, order):
  # This constructs the KML document from the CSV file.
  kmlDoc = xml.dom.minidom.Document()
  
  kmlElement = kmlDoc.createElementNS('http://www.opengis.net/kml/2.2', 'kml')
  kmlElement.setAttribute('xmlns','http://www.opengis.net/kml/2.2')
  kmlElement = kmlDoc.appendChild(kmlElement)
  documentElement = kmlDoc.createElement('Document')
  documentElement = kmlElement.appendChild(documentElement)
  
  nameElement = kmlDoc.createElement('name')
  nameElement.appendChild(kmlDoc.createTextNode('Cansat Flight path'))
  documentElement.appendChild(nameElement)
  
  styleIdElement = kmlDoc.createElement('Style')
  styleIdElement.setAttribute('id','hiker-icon')
  documentElement.appendChild(styleIdElement)
  
  iconStyleElement = kmlDoc.createElement('IconStyle')
  styleIdElement.appendChild(iconStyleElement)
  
  iconElement = kmlDoc.createElement('Icon')
  iconStyleElement.appendChild(iconElement)
  
  hrefElement = kmlDoc.createElement('href')
  hrefElement.appendChild(kmlDoc.createTextNode('http://maps.google.com/mapfiles/kml/pal4/icon49.png'))
  iconElement.appendChild(hrefElement)
  
  hotSpotElement = kmlDoc.createElement('hotSpot')
  hotSpotElement.setAttribute('x','0')
  hotSpotElement.setAttribute('y','.5')
  hotSpotElement.setAttribute('xunits','fraction')
  hotSpotElement.setAttribute('yunits','fraction')
  iconStyleElement.appendChild(hotSpotElement)
  
  

  # Skip the header line.
  csvReader.next()
  
  for row in csvReader:
    placemarkElement = createPlacemark(kmlDoc, row, order)
    documentElement.appendChild(placemarkElement)
  kmlFile = open(fileName, 'w')
  kmlFile.write(kmlDoc.toprettyxml('  ', newl = '\n', encoding = 'utf-8'))

def main():

  
  if len(sys.argv) >1: order = sys.argv[1].split(',')
  else: order = ['date','time','time_index','mode','longitude','latitude','altitude','numsat','speed','cource','bearing','distance','controlangle']
  filename='20180623log'
  csvreader = csv.DictReader(open(filename+'.csv'),order)
  kml = createKML(csvreader, filename+'.kml', order)
if __name__ == '__main__':
  main()