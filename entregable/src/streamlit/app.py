from asyncio import threads
from time import sleep
from warnings import filters
import streamlit as st
import pandas as pd
import imageio.v2 as imageio
import numpy as np
from PIL import Image
from io import BytesIO
import matplotlib.pyplot as plt
import pickle
import os
import cv2
from streamlit_drawable_canvas import st_canvas
st.set_option('deprecation.showPyplotGlobalUse', False)


def show_image():
    img = imageio.imread('../out/salida.ppm')
    plt.imshow(img)
    st.pyplot()

def run_filters(c, b, n, blur, bw, brillo, contraste):
    filters = ''
    primerParametro = ''
    threads = f'{n} '
    if blur:
        filters += 'boxBlur '
        primerParametro += '0 '
    if bw:
        filters += 'blackWhite '
        primerParametro += '0 '
    if brillo:
        filters += 'brightness '
        primerParametro += f'{b} '
    if contraste:
        filters += 'contrast '
        primerParametro += f'{c} '

    filters = filters[:-1]
    primerParametro = primerParametro[:-1]
    cmd = 'c:\cygwin64\bin\mintty.exe Desktop/Tp-2-Filtro-de-imagenes/entregable/src/main' + ' "' + filters  +  '" ' +  threads +  ' "' + primerParametro +  '" ' + '../imgs/ashitaka.ppm ../out/salida.ppm' + " 0"
    os.system(cmd)
   
# Sidebar
st.sidebar.header('Parámetros')

blur = st.sidebar.checkbox('Box blur',value=False)
bw = st.sidebar.checkbox('BlackWhite',value=False)
brillo = st.sidebar.checkbox('Brightness',value=False)
contraste = st.sidebar.checkbox('Contrast',value=False)
c = None
b = None


if brillo:    
    b = st.sidebar.slider('Brillo', -100, 100, 0, 1, '%d')/100
if contraste:
    c = st.sidebar.slider('Contraste',-100, 100, 0, 1, '%d')

n = st.sidebar.slider('Cantidad de threads',1 , 32, 1, 1, '%d')



if st.sidebar.button('Aplicar'):
    run_filters(c, b, n, blur, bw, brillo, contraste)
    #sleep(3)
    #show_image()

# Main layout
st.title('Filtros de imagenes')

st.subheader('Visualizador')

show_image()
