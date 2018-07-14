#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Mon Jul  9 22:14:01 2018

"""


#import geocoding_for_kml
import csv
import xml.dom.minidom
import sys
import datetime


def extractCoord(row):
  # This extracts an address from a row and returns it as a string. This requires knowing
  # ahead of time what the columns are that hold the address information.
  return '%s,%s,%s' % (row['longitude'], row['latitude'], row['altitude'])

def extractTimeStamp(row):
    date_csv=row['date']
    time_csv=row['time']
    if (len(date_csv)<6):
        date_csv='0'+date_csv
    if (len(time_csv)<6):
        time_csv='0'+time_csv  
    datetimes=datetime.datetime(int('20'+date_csv[4:6]),int(date_csv[2:4]),int(date_csv[:2]),int(time_csv[:2]),int(time_csv[2:4]),int(time_csv[4:6]))
    date_timestamp=datetimes.date().strftime('%Y-%m-%d'+'T')
    time_timestamp=datetimes.time().strftime('%H:%M:%S'+'Z')
    return date_timestamp+time_timestamp


def extractTimeStampPlus1(row):
    date_csv=row['date']
    time_csv=row['time']
    if (len(date_csv)<6):
        date_csv='0'+date_csv
    if (len(time_csv)<6):
        time_csv='0'+time_csv   
    datetimes=datetime.datetime(int('20'+date_csv[4:6]),int(date_csv[2:4]),int(date_csv[:2]),int(time_csv[:2]),int(time_csv[2:4]),int(time_csv[4:6]))
    datetimesp1=datetimes+datetime.timedelta(0,1)
    date_timestamp=datetimesp1.date().strftime('%Y-%m-%d'+'T')
    time_timestamp=datetimesp1.time().strftime('%H:%M:%S'+'Z')
    return date_timestamp+time_timestamp


def createPlacemark(kmlDoc, row, order,prev_coord):
  # This creates a  element for a row of data.
  # A row is a dict.
  placemarkElement = kmlDoc.createElement('Placemark')
  
  
  timeStampElement = kmlDoc.createElement('TimeStamp')
  placemarkElement.appendChild(timeStampElement)  
  
  whenElement = kmlDoc.createElement('when')
  whenElement.appendChild(kmlDoc.createTextNode(extractTimeStamp(row)))
  timeStampElement.appendChild(whenElement)

  '''
  timeSpanElement = kmlDoc.createElement('TimeSpan')
  placemarkElement.appendChild(timeSpanElement)  
  
  beginElement = kmlDoc.createElement('begin')
  beginElement.appendChild(kmlDoc.createTextNode(extractTimeStamp(row)))
  timeSpanElement.appendChild(beginElement)  

  endElement = kmlDoc.createElement('end')
  endElement.appendChild(kmlDoc.createTextNode(extractTimeStampPlus1(row)))
  timeSpanElement.appendChild(endElement)    
  '''
  
  styleUrlElement = kmlDoc.createElement('styleUrl')
  styleUrlElement.appendChild(kmlDoc.createTextNode('#hiker-icon'))
  placemarkElement.appendChild(styleUrlElement)
  
  pointElement = kmlDoc.createElement('Point')
  placemarkElement.appendChild(pointElement)
  
  coord = extractCoord(row)
  coorElement = kmlDoc.createElement('coordinates')
  coorElement.appendChild(kmlDoc.createTextNode(coord))
  pointElement.appendChild(coorElement)
  
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
  
  coordinatesElement = kmlDoc.createElement('coordinates')

  coordinatesElement.appendChild(kmlDoc.createTextNode(prev_coord))
  coordinatesElement.appendChild(kmlDoc.createTextNode(extractCoord(row)))
  lineStringElement.appendChild(coordinatesElement)    
  
  
  return placemarkElement,coord

def createKML(csvReader, fileName, order):
  # This constructs the KML document from the CSV file.
  kmlDoc = xml.dom.minidom.Document()
  
  kmlElement = kmlDoc.createElementNS('http://www.opengis.net/kml/2.2', 'kml')
  kmlElement.setAttribute('xmlns','http://www.opengis.net/kml/2.2')
  kmlElement.setAttribute('xmlns:gx','http://www.google.com/kml/ext/2.2')  
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

  # Skip the header line.
  csvReader.next()
  coord=""
  for row in csvReader:
    placemarkElement,coord= createPlacemark(kmlDoc, row, order,coord)
    documentElement.appendChild(placemarkElement)
  kmlFile = open(fileName, 'w')
  kmlFile.write(kmlDoc.toprettyxml('  ', newl = '\n', encoding = 'utf-8'))

def main():

  
  if len(sys.argv) >1: order = sys.argv[1].split(',')
  else: order = ['date','time','time_index','mode','longitude','latitude','altitude','numsat','speed','cource','bearing','distance','controlangle']
  filename='20180623log'
  csvreader = csv.DictReader(open(filename+'.csv'),order)
  kml = createKML(csvreader, filename+'_timestamp.kml', order)
if __name__ == '__main__':
  main()