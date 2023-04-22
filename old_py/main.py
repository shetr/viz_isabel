from direct.showbase.ShowBase import ShowBase
from panda3d.core import TextureStage
from panda3d.core import TexGenAttrib
from panda3d.core import Texture

import numpy as np

temperature = [[[0, 0, 0], [0, 0, 0], [0, 0, 0]], [[0, 0, 0], [0, 0, 0], [0, 0, 0]], [[0, 0, 0], [0, 0, 0], [0, 0, 0]]]

class MyApp(ShowBase):

    def __init__(self):
        ShowBase.__init__(self)

        temps = np.random.rand(3,3,3)

        self.teapot = self.loader.loadModel('teapot.egg')
        self.teapot.reparentTo(self.render)
        self.teapot.setScale(0.25, 0.25, 0.25)
        self.teapot.setPos(0, 8, 0)
        # Apply scale and position transforms on the model.
        self.teapot.setTexGen(TextureStage.getDefault(), TexGenAttrib.MWorldPosition)
        self.teapot.setTexProjector(TextureStage.getDefault(), self.render, self.teapot)
        self.teapot.setTexPos(TextureStage.getDefault(), 0.44, 0.5, 0.2)
        self.teapot.setTexScale(TextureStage.getDefault(), 0.2)

        self.temps_tex = Texture("temps")
        self.temps_tex.setup3dTexture(3, 3, 3, 2, 3)
        
        #self.tex = self.loader.load3DTexture('woodgrain-#.png')
        #self.teapot.setTexture(self.tex)


if __name__ == "__main__":

    app = MyApp()
    app.run()


        