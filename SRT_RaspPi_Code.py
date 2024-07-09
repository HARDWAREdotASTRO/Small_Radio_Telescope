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
#ser2 = serial.Serial('/dev/ttyACM0', baudrate = 9600, timeout=1)

#Dummy variables for keeping track of button presses.
prevclick = 0
homeprev = 0
altforprev = 0
altrevprev = 0
azforprev = 0
azrevprev = 0
delay1 = 0
#Blank string to be updated for updating the realtime location of the gear/dish
motAz = ""

# This is a list of commands in a seperate file online that my program would 
# automatically pull commands from. It will hopefully be replaced with
# one I'm having a comp sci guy produce.
external_stylesheets = ['https://codepen.io/chriddyp/pen/bWLwgP.css']

# This is to shorten the command needed to reference the CSS file as well as the Dash libraries
app = dash.Dash(__name__, external_stylesheets=external_stylesheets)

# This is where the darkness begins.   
app.layout = html.Div([
    #Start with the banner at the top of the page
    html.Div(
            id="container",
            style={"background-color": "#4B08A1",
                  "color": "white"},
            children=[
                    html.H3("Small Radio Telescope Control"),
                #Something goes wrong here, Im trying to put the Winona State symbol in that links to the WSU homepage. It works, its just not in the upper right 
#                    html.A(
#                        html.Img(src=
#                                 "https://mfr.osf.io/export?url=https://osf.io/xa6p8/?action=download%26mode=render%26direct%26public_file=False&initialWidth=675&childId=mfrIframe&parentTitle=OSF+%7C+NewWinona22.PNG&parentUrl=https://osf.io/xa6p8/&format=2400x2400.jpeg"                            ),
#                    href="https://www.winona.edu/",
#                        )
            ],
        className="banner"
    ),
    #Setup for the graph to display objects seen from Earth
    html.Div([
        #Dropdown bar to choose which planet will be selected
        html.Div([
            dcc.Dropdown(
                #ID for callback function
                id='solarsystem',
                #Dropdown selection options
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
                value='sun'
            )
        ]
        ),
        #Initialize graph and id for callback
        html.Div([
            dcc.Graph(
                id='graph'
            )
        ],
            className="twelve columns",
            style={
                "marginTop": "5%"
            }
        )
    ],
        #Take up the full width of the page
        className='twelve columns'
    ),
    #Seperate the rest of the page
    html.Div([
        #Direct control compartment of the page
        html.Div([
                html.Div([
                    #Title
                    html.H3(
                        "Direct Control"
                    )
                ], 
                    className='Title'
                ),
            #Two buttons per row
                html.Div([
                     daq.StopButton(
                            id="stop-button",
                            buttonText="STOP",
                            style={
                                #Not entirely sure how these work, padding helpes seperate boxes, maybe
                                "display": "flex",
                                "justify-content": "space-around",
                                "padding": "10px 10px 10px 10px"
                            },
                             #Six columns = half the row
                            className="six columns",
                            n_clicks=0
                        ),
                    daq.StopButton(
                            id="go-home-button",
                            buttonText="Go Home",
                            style={
                                "display": "flex-right",
                                "justify-content": "space-around",
                                "padding": "10px 10px 10px 10px"
                            },
                            className="six columns",
                            n_clicks=0
                        )
                ],
                    style={
                        #Box shadow gives a light border
                        "align-items": "center",
                        'boxShadow': '1px 1px 1px 1px rgba(204,204,204,0.4)',
                        "padding": "10px 10px 10px 20px"
                    },
                    #Take up the whole row
                    className="row"
                ),
            #Seperate the motor controls for each motor
                html.Div([
                    html.Div([
                        html.H5(
                            "Altitude Motor"
                        )
                    ], 
                        className='Title'
                    ),
                    html.Div([
                        daq.StopButton(
                            id="alt-forward-button",
                            buttonText="Forward",
                            style={
                                "display": "flex",
                                "justify-content": "space-around",
                                "align-items": "center",
                                "padding": "10px 10px 10px 10px"
                            },
                            className="six columns",
                            n_clicks=0
                        ),
                        daq.StopButton(
                            id="alt-reverse-button",
                            buttonText="Reverse",
                            style={
                                "display": "flex",
                                "justify-content": "space-around",
                                "align-items": "flex-end",
                                "padding": "10px 10px 10px 10px"
                            },
                            className="six columns",
                            n_clicks=0
                        )
                    ],
                        className="row"
                    ),
                    html.Div([
                        #A slider to control the motor speed
                        dcc.Slider(
                            id="alt-slider",
                            min=0,
                            max=100,
                            value=50
                        ),
                        html.Div(
                            id='speed-control-alt')
                        
                    ],
                        style={
                        "padding": "10px 10px 10px 20px"
                    },
                        className="row"
                    )
                ],
                    style={
                        "align-items": "center",
                        'boxShadow': '1px 1px 1px 1px rgba(204,204,204,0.4)',
                        "padding": "10px 10px 10px 20px"
                    },
                        className="row"
                ),
                html.Div([
                    html.Div([
                        html.H5(
                            "Azimuth Motor"
                        )
                    ], 
                        className='Title'
                    ),
                    html.Div([
                        daq.StopButton(
                            id="az-forward-button",
                            buttonText="Forward",
                            style={
                                "display": "flex",
                                "justify-content": "space-around",
                                "align-items": "center",
                                "padding": "10px 10px 10px 10px"
                            },
                            className="six columns",
                            n_clicks=0
                        ),
                        daq.StopButton(
                            id="az-reverse-button",
                            buttonText="Reverse",
                            style={
                                "display": "flex",
                                "justify-content": "space-around",
                                "align-items": "flex-end",
                                "padding": "10px 10px 10px 10px"
                            },
                            className="six columns",
                            n_clicks=0
                        )
                    ],
                        className="row"
                    ),
                    html.Div([
                        dcc.Slider(
                            id="az-slider",
                            min=0,
                            max=100,
                            value=50
                        ),
                        html.Div(
                            id='speed-control-az')
                        
                    ],
                        style={
                        "padding": "10px 10px 10px 20px"
                    },
                        className="row"
                    )
                ],
                    style={
                        "align-items": "center",
                        'boxShadow': '1px 1px 1px 1px rgba(204,204,204,0.4)',
                        "padding": "10px 10px 10px 20px"
                    },
                        className="row"
                ),
            ],
                style={
                    "align-items": "center",
                    "border": "1px solid #2a3f5f",
                    "border-radius": "4px",
                    #'boxShadow': '0px 0px 5px 5px rgba(204,204,204,0.4)',
                    "padding": "10px 10px 10px 20px"
                },
                #Direct control box takes a third of the page
                className="four columns"
            ),
            html.Div([
                #A box to callback the srt's current direction
                html.Div([
                    html.Div([
                        html.H3(
                            "SRT Direction"
                        )
                    ], 
                        className='Title'
                    ),
                    html.Div([
                        html.Div([
                            html.Div([
                                "Altitude:  "
                            ],
                                style={
                                    'textAlign': 'right'
                                },
                                className="three columns"
                            ),
                            html.Div(
                                id="altitude",
                                className="three columns",
                                style={
                                    'marginRight': '20px'
                                }
                            )
                        ], 
                            className="twelve columns"
                        ),
                        html.Div([
                            html.Div([
                                "Azimuth:  "
                            ],
                                style={
                                    'textAlign': 'right'
                                },
                                className="three columns"
                            ),
                            html.Div(
                                id="azimuth",
                                className="three columns",
                                style={
                                    'marginRight': '20px'
                                }
                            )
                        ], 
                            className="twelve columns"
                        )
                    ],
                        style={
                            "align-items": "center",
                            'boxShadow': '1px 1px 1px 1px rgba(204,204,204,0.4)',
                            "padding": "10px 10px 10px 20px"
                        },
                        className="twelve columns"
                    )
                ],
                    #Gets half the row, shares with Input direction
                    className="six columns"
                ),
                html.Div([
                    html.Div([
                        html.H3(
                            "Input Direction"
                        )
                    ], 
                        className='Title'
                    ),
                    html.Div([
                        #Reports alt az of ra and dec user input
                        html.Div([
                            html.Div([
                                "Altitude:  "
                            ],
                                style={
                                    'textAlign': 'right'
                                },
                                className="three columns"
                            ),
                            html.Div(
                                id="alt",
                                className="three columns",
                                style={
                                    'marginRight': '20px'
                                }
                            )
                        ], 
                            className="twelve columns"
                        ),
                        html.Div([
                            html.Div(
                                "Azimuth:   ",
                                style={
                                    'textAlign': 'right'
                                },
                                className="three columns"
                            ),
                            html.Div(
                                id="az",
                                className="four columns",
                                style={
                                    'marginRight': '20px'
                                }
                            )
                        ], 
                            className="twelve columns"
                        )
                    ],
                        style={
                            "align-items": "center",
                            'boxShadow': '1px 1px 1px 1px rgba(204,204,204,0.4)',
                            "padding": "10px 10px 10px 20px"
                        },
                        className="twelve columns"
                    )
                ],
                    className="six columns"
                )                    
            ],
                style={
                    "align-items": "center",
                    "border": "1px solid #2a3f5f",
                    "border-radius": "4px",
                    #'boxShadow': '0px 0px 5px 5px rgba(204,204,204,0.4)',
                    "padding": "20px 20px 20px 20px"
                },
                className="eight columns"
            ),
            html.Div([
                #User input object
                html.Div([
                        html.H3(
                            "Object"
                        )
                ], 
                    className='Title'
                ),
                html.Div([
                    dcc.Input(
                        id='RA', 
                        value='24d20m30s', 
                        type='text',
                        className="ten columns"
                    ),
                    html.H5(
                        "Right Ascension", 
                        style={
                            "textAlign": "bottom"
                        }
                    )
                ], 
                    style={
                        "align-items": "center",
                        'boxShadow': '1px 1px 1px 1px rgba(204,204,204,0.4)',
                        "padding": "10px 10px 10px 20px"
                    },
                    className='five columns'
                ),

                html.Div([
                    dcc.Input(
                        id='DEC', 
                        value='12d24m35s', 
                        type='text',
                        className="ten columns"
                    ),
                    html.H5(
                        "Declenation", 
                        style={
                            "textAlign": "bottom"
                        }
                    )
                ],
                    style={
                        "align-items": "center",
                        'boxShadow': '1px 1px 1px 1px rgba(204,204,204,0.4)',
                        "padding": "10px 10px 10px 20px"
                    },
                    className='five columns'
                ),
                html.Div([
                    daq.StopButton(
                        id="go-button",
                        buttonText="Go",
                        style={
                            "display": "flex",
                            "justify-content": "center",
                            "align-items": "flex-end"
                        },
                            className="three columns",
                            n_clicks=0
                    )
                ],                
                    style={
                        "align-items": "center",
                        
                        "padding": "10px 10px 10px 20px"
                    },
                    className="twelve columns"
                )
            ],
                style={
                    "align-items": "center",
                    "border": "1px solid #2a3f5f",
                    "border-radius": "4px",
                    "position": "relative",
                    "marginTop": "6%",
                    "marginBottom": "4%",
                    "padding": "10px 10px 10px 10px"
                    },
                className="eight columns"
            ),
        html.Div([
            html.Div([
                dcc.Textarea(
                    id="status-monitor",
                    placeholder=" ",
                    value="",
                    style={
                        "width": "89%",
                        "height": "157px",
                        "marginLeft": "5.7%",
                        "marginBottom": "6%",
                    },
                ),
            ],
                className="four columns",
                style={
                    "marginTop": "5%"
                }
            ),
                html.Div([
                    #Select how to observe
                    html.Div([
                            html.H3(
                                "Observing Method"
                            )
                        ], 
                            className='Title'
                        ),
                        html.Div([
                            html.Div([
                                html.H6(
                                    "Method Select"
                                )
                            ], 
                                className='Title'
                            ),
                            #Pick the method
                            dcc.RadioItems(
                                id="select",
                                options=[
                                    {'label': 'Go To', 'value': 'Goto'},
                                    {'label': 'Tracking', 'value': 'Track'},
                                    {'label': 'Scan', 'value': 'Scan'}
                                ],
                                value='Goto'
                            )
                        ],
                            style={
                                "align-items": "center",
                                'boxShadow': '1px 1px 1px 1px rgba(204,204,204,0.4)',
                                "padding": "10px 10px 5px 10px"
                            },
                            className="twelve columns"
                        ),
                    html.Div([
                        #Boxes for specific method instructions
                        html.Div([
                            html.Div([
                                html.Div([
                                    html.H6(
                                        "Scan"
                                    )
                                ], 
                                    className='Title'
                                ),
                                dcc.RadioItems(
                                    id='scanner',
                                    options=[
                                        {'label': 'Full Sky', 'value': 'FullSky'},
                                        {'label': 'Object', 'value': 'Obj'}
                                    ],
                                    value='Obj'
                                )
                            ],
                                className="four columns"
                            ),
                            html.Div([
                                html.Div([
                                    html.H6(
                                        "Box Size"
                                    )
                                ], 
                                    className='Title'
                                ),
                                html.Div([
                                    dcc.Input(
                                        id='boxSize', 
                                        value='10', 
                                        type='text',
                                        className="five columns"
                                    ),
                                    html.H5(
                                        "°", 
                                        style={
                                            "paddingRight": "50%",
                                            "textAlign": "right"
                                        }
                                    )
                                ]
                                )
                        ],
                            className="four columns"
                        ),
                        html.Div([
                            html.Div([
                                html.H6(
                                    "Scan Speed"
                                )
                            ], 
                                className='Title'
                            ),
                            html.Div([
                                dcc.Input(
                                    id='scanSpeed', 
                                    value='10', 
                                    type='text',
                                    className="five columns"
                                ),
                                html.H5(
                                    "°/min", 
                                    style={
                                        "paddingRight": "30%",
                                        "textAlign": "right"
                                    }
                                )
                            ], 
                                className='row'
                            )
                    ],
                        className="four columns"
                    )
                    ],
                        style={
                            "align-items": "center",
                            'boxShadow': '1px 1px 1px 1px rgba(204,204,204,0.4)',
                            "padding": "10px 10px 10px 10px"
                        },
                        className="row"
                    )
                    ]
                    )
                ],
                    style={
                        "align-items": "center",
                        "border": "1px solid #2a3f5f",
                        "border-radius": "4px",
                        "padding": "10px 10px 10px 10px"
                        },
                    className="eight columns"
                )
        ],
            className="twelve columns"
            #style={
            #    "marginTop": "3%"
            #}
        ),
            html.Div([
                html.Div(id='go-home-button-count'),
                html.Div(id='stop-button-count'),
                html.Div(id='alt-for-button-count'),
                html.Div(id='alt-rev-button-count'),
                html.Div(id='az-for-button-count'),
                html.Div(id='az-rev-button-count'),
                html.Div(id='placeholder'),
                dcc.RadioItems(
                    id='placeholder2',
                    options=[
                        {'label': 'Place', 
                         'value': 'Place'}
                    ],
                    value='Place'
                ),
                dcc.Interval(
                    id='refresher',
                    interval=1000),
                dcc.Interval(
                    id='refresher2',
                    interval=10000)
            ],
                style={
                    "visibility": "hidden"
                }
            )
    ],
        className="row"
    ),
    ],
        style={
            'padding': '0px 10px 15px 10px',
            'marginLeft': 'auto', 
            'marginRight': 'auto',
            "width": "900px",
            'boxShadow': '0px 0px 5px 5px rgba(204,204,204,0.4)'
        }

)
