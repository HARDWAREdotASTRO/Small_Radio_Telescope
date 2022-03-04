import dash
import dash_core_components as dcc
import dash_html_components as html
import dash_daq as daq
from dash.dependencies import Input, Output, State

from astropy.coordinates import EarthLocation
from astropy import units as u
from astropy.coordinates import SkyCoord
from astropy.coordinates import AltAz
from astropy.coordinates import get_body
from astropy.time import Time
from astropy.table import Table

import numpy as np
import matplotlib.pyplot as plt
import plotly.graph_objs as go

from IPython.display import Image
from astropy.visualization import astropy_mpl_style
plt.style.use(astropy_mpl_style)


import time
import serial

#Initialize serial communication, this is often commented out to see the webpage when the Arduino is not hooked up
#ser1 = serial.Serial('/dev/ttyACM0', baudrate = 9600, timeout=1)



#Track button presses.
# array defined as follows [prevClick, homeprev, altforprev, altrevprev, azforprev, azrevprev, delay1]
buttons = np.zeros(7)

#Looks like this is supposed to be real time data here. JSON format api is most likely culprit.
motAz = ""


#Talking about a list of commands here. Need to find out what that is. 

external_stylesheets = ['https://codepen.io/chriddyp/pen/bWLwgP.css']

#Begin defining the app here. Oh boy, exciting...
app = dash.Dash(__name__, external_stylesheets= external_stylesheets)

app.layout = html.Div([
    ###########################################
    ######Part 1: Banner starts here###########
    ###########################################

    html.Div(
        id = "container", 
        style = {"background_color": "#4B08A1",
                "color": "white"},
                children = [
                    html.H3("Small Radio Telescope Control"),
                    html.A(html.Img(src = "C:/Users/JonHasan/Downloads/Winona_State_University_logo.jpg"),
                    href = "https://www.winona.edu/",)
                ],
        className = "banner"
    ),

    ###########################################
    ######Part 1: Banner ends here###########
    ###########################################


    ###########################################
    ##Part 2: Dropdown and Graph starts here###
    ###########################################

    html.Div([ 
        #dropwon to choose whichplanet
        dcc.Dropdown(
            #ID for callback
            id = 'solarsystem',
            #dropdown selections
            options=[
                {'label': 'Object', 'value': 'object'},
                {'label': 'Sun', 'value': 'sun'},
                {'label': 'Moon', 'value': 'moon'},
                {'label': 'Mercury', 'value': 'mercury'},
                {'label': 'Venus', 'value': 'venus'},
                {'label': 'Mars', 'value': 'mars'},
                {'label': 'Jupiter', 'value': 'jupiter'},
                {'label': 'Saturn', 'value': 'saturn'},
                {'label': 'Uranus', 'value': 'uranus'},
                {'label': 'Neptune', 'value': 'neptune'},
            ],
            #initial value 
            value = 'sun'
        ),
        #initialize graph and id for callback
        dcc.Graph(
            id = 'graph'
        )
    ],

    #Have the graph take up entirety of webpage 
    className = 'twelve columns '
    ),
    ###########################################
    ##Part 2: Dropdown and Graph ends here#####
    ###########################################

    #Bulk of the GUI begins here
    html.Div([

    ###########################################
    ##Part 3: Direct Control starts here  #####
    ###########################################
        html.Div([ 

            html.H3("Direct Control"
            )
        
        ],
        className = 'Title'
        ),

        #Two buttons per row - Clayton Hanson
        html.Div([
            daq.StopButton(
                id = "stop-button",
                buttonText = "STOP",
                style = {
                    #flex layout used for dynamic content; will grow with number of children items,
                    #  justify content is a property of flex layout, alligns items in a line with equal spacing
                    # padding is the spacing around each item, Clayton stated this before in AppV9.py. 
                    "display": "flex",
                    "justify-content": "space-around",
                    "padding": "10px 10px 10px 10px"
                },
                # Cover Six columns worth of space
                className = "six columns",
                n_clicks = 0
            ),
            daq.StopButton(
                id = "go-home-button",
                buttonText = "Go Home",
                style = {
                    "display": "flex-right",
                    "justify-content": "space-around",
                    "padding": "10px 10px 10px 10px"
                },
                className = "six columns",
                n_clicks = 0
            )

        ],
            style = {
                "align-items": "center",
                'boxShadow': '1px 1px 1px 1px rgba(204,204,204,0.4)',
                "padding": "10px 10px 10px 20px"
            },

            className = "row"
        ),

    ###########################################
    ##Part 3: Direct Control ends here  #####
    ###########################################

    ])



])

if __name__ == '__main__':
    app.run_server(debug=True)
