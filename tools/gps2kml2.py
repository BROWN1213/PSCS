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



def createKML(csvReader, fileName, order):
  # This constructs the KML document from the CSV file.
  kmlDoc = xml.dom.minidom.Document()
  
  kmlElement = kmlDoc.createElementNS('http://www.opengis.net/kml/2.2', 'kml')
  kmlElement.setAttribute('xmlns','http://www.opengis.net/kml/2.2')
  kmlElement = kmlDoc.appendChild(kmlElement)
  documentElement = kmlDoc.createElement('Document')
  documentElement = kmlElement.appendChild(documentElement)
  
  nameElement = kmlDoc.createElement('name')
  nameElement.appendChild(kmlDoc.createTextNode('Cansat Flight 3D path'))
  documentElement.appendChild(nameElement)
  
  styleIdElement = kmlDoc.createElement('Style')
  styleIdElement.setAttribute('id',"yellowLineGreenPoly")
  documentElement.appendChild(styleIdElement)
  
  lineStyleElement = kmlDoc.createElement('LineStyle')
  styleIdElement.appendChild(lineStyleElement)
  
  colorElement = kmlDoc.createElement('color')
  colorElement.appendChild(kmlDoc.createTextNode('7f00ffff'))
  lineStyleElement.appendChild(colorElement)

  widthElement = kmlDoc.createElement('width')
  widthElement.appendChild(kmlDoc.createTextNode('4'))
  lineStyleElement.appendChild(widthElement)
  
  polyStyleElement = kmlDoc.createElement('PolyStyle')
  styleIdElement.appendChild(polyStyleElement)
  
  colorElement = kmlDoc.createElement('color')
  colorElement.appendChild(kmlDoc.createTextNode('7f00ff00'))
  polyStyleElement.appendChild(colorElement)  
 
    
  placemarkElement = kmlDoc.createElement('Placemark')
  documentElement.appendChild(placemarkElement)
  
  styleUrlElement = kmlDoc.createElement('styleUrl')
  styleUrlElement.appendChild(kmlDoc.createTextNode('#yellowLineGreenPoly'))
  placemarkElement.appendChild(styleUrlElement)  
  
  lineStringElement = kmlDoc.createElement('LineString')
  placemarkElement.appendChild(lineStringElement)  
  
  extrudeElement = kmlDoc.createElement('extrude')
  extrudeElement.appendChild(kmlDoc.createTextNode('1'))
  lineStringElement.appendChild(extrudeElement)   
  
  tessellateElement = kmlDoc.createElement('tessellate')
  tessellateElement.appendChild(kmlDoc.createTextNode('1'))
  lineStringElement.appendChild(tessellateElement) 
  
  altitudeModeElement = kmlDoc.createElement('altitudeMode')
  altitudeModeElement.appendChild(kmlDoc.createTextNode('absolute'))
  lineStringElement.appendChild(altitudeModeElement)   

  # Skip the header line.  
  csvReader.next()
  
  coordinatesElement = kmlDoc.createElement('coordinates')
  
  for row in csvReader:
    coordinatesElement.appendChild(kmlDoc.createTextNode(extractCoord(row)))
    
  lineStringElement.appendChild(coordinatesElement)   
  


  kmlFile = open(fileName, 'w')
  kmlFile.write(kmlDoc.toprettyxml('  ', newl = '\n', encoding = 'utf-8'))

def main():

  
  if len(sys.argv) >1: order = sys.argv[1].split(',')
  else: order = ['date','time','time_index','mode','longitude','latitude','altitude','numsat','speed','cource','bearing','distance','controlangle']
  filename='20180623log'
  csvreader = csv.DictReader(open(filename+'.csv'),order)
  kml = createKML(csvreader, filename+'_3D.kml', order)
if __name__ == '__main__':
  main()