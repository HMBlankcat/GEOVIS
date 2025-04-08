#include "Dem.h"

PointZ terri[nrows * ncols];
void myInit() {
    glEnable(GL_DEPTH_TEST);
    SetupLights();
    glClearColor(0.0, 0.0, 0.0, 1.0);
}

void convertDemTo3DPoints(PointZ terri[nrows * ncols]) {
    for (int i = 0; i < nrows; i++) {
        for (int j = 0; j < ncols; j++) {
            terri[i * ncols + j].x = (xllcorner + j * CELLSIZE) / 1000.0f;
            terri[i * ncols + j].y = (yllcorner + (nrows - 1 - i) * CELLSIZE) / 1000.0f;
            terri[i * ncols + j].h = dem[i][j] / 1000.0f; // assuming elevation is also in meters
        }
    }
}
void OnReshape(int w, int h) {
    WinWidth = w;
    WinHeight = h;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.0, (GLdouble)w / (GLdouble)h, 0.1, 10000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, -1500.0, 2000.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void OnDisplay(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0, -1500.0, 2000.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); // 更新相机位置

    glTranslatef(T_x, T_y, T_z);
    glRotatef(R_x, 1.0, 0.0, 0.0);
    glRotatef(R_y, 0.0, 1.0, 0.0);
    glRotatef(R_z, 0.0, 0.0, 1.0);

    drawTerrain(terri, Mode);

    glutSwapBuffers();
}

void drawTerrain(PointZ terri[], int mode) {
    switch (mode) {
    case WIRE: {
        glColor4f(0, 1, 0, 1);
        for (int i = 0; i < nrows - 1; i++) {
            for (int j = 0; j < ncols - 1; j++) {
                glBegin(GL_LINE_LOOP);
                glVertex3f(terri[i * ncols + j].x, terri[i * ncols + j].h, terri[i * ncols + j].y);
                glVertex3f(terri[(i + 1) * ncols + j].x, terri[(i + 1) * ncols + j].h, terri[(i + 1) * ncols + j].y);
                glVertex3f(terri[(i + 1) * ncols + (j + 1)].x, terri[(i + 1) * ncols + (j + 1)].h, terri[(i + 1) * ncols + (j + 1)].y);
                glEnd();

                glBegin(GL_LINE_LOOP);
                glVertex3f(terri[(i + 1) * ncols + (j + 1)].x, terri[(i + 1) * ncols + (j + 1)].h, terri[(i + 1) * ncols + (j + 1)].y);
                glVertex3f(terri[i * ncols + (j + 1)].x, terri[i * ncols + (j + 1)].h, terri[i * ncols + (j + 1)].y);
                glVertex3f(terri[i * ncols + j].x, terri[i * ncols + j].h, terri[i * ncols + j].y);
                glEnd();
            }
        }
        break;
    }
    case SOLID: {
        glColor3f(0.5, 0.5, 0.5);
        for (int i = 0; i < nrows - 1; i++) {
            for (int j = 0; j < ncols - 1; j++) {
                glBegin(GL_TRIANGLES);
                // First triangle
                glVertex3f(terri[i * ncols + j].x, terri[i * ncols + j].h, terri[i * ncols + j].y);
                glVertex3f(terri[(i + 1) * ncols + j].x, terri[(i + 1) * ncols + j].h, terri[(i + 1) * ncols + j].y);
                glVertex3f(terri[(i + 1) * ncols + (j + 1)].x, terri[(i + 1) * ncols + (j + 1)].h, terri[(i + 1) * ncols + (j + 1)].y);

                // Second triangle
                glVertex3f(terri[(i + 1) * ncols + (j + 1)].x, terri[(i + 1) * ncols + (j + 1)].h, terri[(i + 1) * ncols + (j + 1)].y);
                glVertex3f(terri[i * ncols + (j + 1)].x, terri[i * ncols + (j + 1)].h, terri[i * ncols + (j + 1)].y);
                glVertex3f(terri[i * ncols + j].x, terri[i * ncols + j].h, terri[i * ncols + j].y);
                glEnd();
            }
        }
        break;
    }
    }
}

void OnColorGrade(float mz) {
    if (isColorGrade) {
        glShadeModel(GL_SMOOTH);
        if (mz < 1.2)
            glColor3f(0, 0.4588, 0.1529);
        else if (mz >= 1.2 && mz < 1.7)
            glColor3f(0.0549, 0.5921, 0.0431);
        else if (mz >= 1.7 && mz < 2.3)
            glColor3f(0.3372, 0.7254, 0.1137);
        else if (mz >= 2.3 && mz < 2.8)
            glColor3f(0.9960, 0.9568, 0.3137);
        else if (mz >= 2.8 && mz < 3.4)
            glColor3f(1.0, 0.8039, 0.1019);
        else if (mz >= 3.4 && mz < 3.9)
            glColor3f(0.8156, 0.5294, 0.1254);
        else if (mz >= 3.9 && mz < 4.4)
            glColor3f(0.6509, 0.4823, 0.3294);
        else if (mz >= 4.4 && mz < 5.0)
            glColor3f(0.6745, 0.5647, 0.4705);
        else if (mz >= 5.0 && mz < 5.6)
            glColor3f(1.0, 1.0, 1.0);
    }
    else {
        glColor3f(0.0, 1.0, 0.0);
    }
}

void createMenu(void) {
    GLint nMainMenu;
    nMainMenu = glutCreateMenu(OnMenu);
    glutAddMenuEntry("WIRE", WIRE);
    glutAddMenuEntry("SOLID", SOLID);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void OnMenu(int value) {
    switch (value) {
    case WIRE:
        Mode = WIRE;
        isColorGrade = true;
        break;
    case SOLID:
        Mode = SOLID;
        isColorGrade = true;
        break;
    }
    OnReshape(WinWidth, WinHeight);
    glutPostRedisplay();
}

void proNormalKey(unsigned char key, int x, int y) {
    switch (key) {
    case 'x':
        R_x -= 0.5f;
        break;
    case 'X':
        R_x += 0.5f;
        break;
    case 'y':
        R_y -= 0.5f;
        break;
    case 'Y':
        R_y += 0.5f;
        break;
    case 'z':
        R_z -= 0.5f;
        break;
    case 'Z':
        R_z += 0.5f;
        break;
    case 'n':
        T_y -= 10.0f;
        break;
    case 'N':
        T_y += 10.0f;
        break;
    case 'l':
        T_x += 10.0f;
        break;
    case 'L':
        T_x -= 10.0f;
        break;
    case 'o':
        T_z += 10.0f;
        break;
    case 'O':
        T_z -= 10.0f;
        break;
    case 'b':
        Over_z += 100;
        break;
    case 'B':
        Over_z -= 100;
        break;
    default:
        break;
    }
    OnReshape(WinWidth, WinHeight);
    glutPostRedisplay();
}

void SetupLights() {
    GLfloat ambientLight[] = { 0.6, 0.6, 0.6, 1.0 };
    GLfloat diffuseLight[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat specularLight[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat lightPos[] = { 10.0, 700.0, 200.0, 1.0 };

    glEnable(GL_LIGHTING);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specularLight);
    glMateriali(GL_FRONT, GL_SHININESS, 100);
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);  // 初始化 GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);  // 设置显示模式
    glutInitWindowSize(600, 500);  // 设置窗口大小
    glutInitWindowPosition(100, 100);  // 设置窗口位置
    glutCreateWindow("三维地形动态显示程序设计");  // 创建窗口

    myInit();  // 调用初始化函数
    glutReshapeFunc(OnReshape);  // 设置窗口调整大小的回调函数
    glutDisplayFunc(OnDisplay);  // 设置绘制回调函数
    glutKeyboardFunc(proNormalKey);  // 设置键盘回调函数
    createMenu();  // 创建右键菜单

    glutMainLoop();  // 进入 GLUT 主循环
    return 0;
}
