#include "utils.h"
// #include <cmath>

#define NUM_POINTS 500000
#define WIDTH 1800
#define HEIGHT 900

std::vector<std::vector<double>> affineTransformations
{
  {0.5, 0.0, 0.0, 0.5, 0.0, 0.0},
  {0.5, 0.0, 0.0, 0.5, 1.28, 0.0},
  {0.5, 0.0, 0.0, 0.5, 0.64, 0.8}
  
  // {0.195, -0.488, 0.344, 0.4430, 0.4431, 0.2452},
  // {0.362, 0.414, -0.252, 0.361, 0.2511, 0.5692},
  // {-0.637, 0, 0, 0.501, 0.8562, 0.2512},
  // {-0.035, 0.07, -0.469, 0.022, 0.4884, 0.5069},
  // {-0.058, -0.07, 0.453, -0.111, 0.5976, 0.0969}

  // {0, 0.2439, 0, 0.3053, 0, 0},
  // {0.7248, 0.0337, -0.0253, 0.7426, 0.2060, 0.2538},
  // {0.1583, -0.1297, 0.355, 0.3676, 0.1383, 0.175},
  // {0.3386, 0.3694, 0.2227, -0.0756, 0.0679, 0.0826}

};

std::vector<double> probabilities;

std::vector<double> points;
bool pointsUpdated = false;

bool isConverging() {
  bool converge = true;
  
  for (int i=0; i<affineTransformations.size(); i++) {
    double a = affineTransformations[i][0];
    double d = affineTransformations[i][3];
    if ((a+d < -1) || (a+d > 1)) {
      std::cout<<"\nTransformation "<<i+1<<" will not converge."<<std::endl;
      converge = false;
    }
  }

  return converge;
}

int generateIndex() {
  double randomNum = rand()/((double)RAND_MAX + 1);
  double randSum = 0;

  for (int i=0; i < probabilities.size(); i++) {
    randSum += probabilities[i];
    if (randomNum <= randSum) {
      return i;
    }
  }

  return probabilities.size() - 1; // If by chance randNum is not less than randSum because randSum does not reach 1 due to approximation errors in computation
}

void generateProbabilities() {
  probabilities.clear();
  double detSum = 0;

  for (int i=0; i<affineTransformations.size(); i++) {
    double a = affineTransformations[i][0];
    double b = affineTransformations[i][1];
    double c = affineTransformations[i][2];
    double d = affineTransformations[i][3];
    
    double det = a*d - b*c;
    if (det < 0) {
      det = -det;
    }

    probabilities.push_back(det);
    detSum += det;
  }

  std::cout << "\nProbabilities:";
  for (int i=0; i<probabilities.size(); i++) {
    probabilities[i] = probabilities[i]/detSum;
    std::cout << " " << probabilities[i];
  }
  std::cout << std::endl;
}

void generatePoints()
{
  generateProbabilities();
  points.clear();
  double point[2] = {0.1, -0.2};
  points.push_back(point[0]);
  points.push_back(point[1]);
  points.push_back(0.0);
  for(int i=0; i<NUM_POINTS; i++) {
    int j = generateIndex();
    // int j = rand()/((double)RAND_MAX + 1)*affineTransformations.size(); // Random index of affineTransformations
    double old_point[2] = {point[0], point[1]};
    point[0] = old_point[0]*affineTransformations[j][0] + old_point[1]*affineTransformations[j][1] + affineTransformations[j][4];
    point[1] = old_point[0]*affineTransformations[j][2] + old_point[1]*affineTransformations[j][3] + affineTransformations[j][5];
    points.push_back(point[0]);
    points.push_back(point[1]); 
    points.push_back(0.0);
  }
  int n = points.size();
}

int main(int, char* argv[]) {
  GLFWwindow* window = setupWindow(WIDTH, HEIGHT);
  ImGuiIO& io = ImGui::GetIO(); // Create IO object

  ImVec4 clear_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

  unsigned int shaderProgram = createProgram("./shaders/vshader.vs", "./shaders/fshader.fs");
  glUseProgram(shaderProgram);

  // Create VBO, VAO
  unsigned int VBO_points;
  unsigned int VAO_points;
  glGenBuffers(1, &VBO_points);
  glGenVertexArrays(1, &VAO_points);

  generatePoints();
  glBindVertexArray(VAO_points);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_points);
  glBufferData(GL_ARRAY_BUFFER, points.size()*sizeof(GLdouble), &points[0], GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), (void*)0);
  glEnableVertexAttribArray(0); //Enable first attribute buffer (vertices)

  // Affine transformations
  static double a1 = 0.0;static double b1 = 0.0;static double c1 = 0.0;static double d1 = 0.0;static double e1 = 0.0;static double f1 = 0.0;
  static double a2 = 0.0;static double b2 = 0.0;static double c2 = 0.0;static double d2 = 0.0;static double e2 = 0.0;static double f2 = 0.0;
  static double a3 = 0.0;static double b3 = 0.0;static double c3 = 0.0;static double d3 = 0.0;static double e3 = 0.0;static double f3 = 0.0;
  static double a4 = 0.0;static double b4 = 0.0;static double c4 = 0.0;static double d4 = 0.0;static double e4 = 0.0;static double f4 = 0.0;
  static double a5 = 0.0;static double b5 = 0.0;static double c5 = 0.0;static double d5 = 0.0;static double e5 = 0.0;static double f5 = 0.0;
  static double a6 = 0.0;static double b6 = 0.0;static double c6 = 0.0;static double d6 = 0.0;static double e6 = 0.0;static double f6 = 0.0;
  static double a7 = 0.0;static double b7 = 0.0;static double c7 = 0.0;static double d7 = 0.0;static double e7 = 0.0;static double f7 = 0.0;
  static double a8 = 0.0;static double b8 = 0.0;static double c8 = 0.0;static double d8 = 0.0;static double e8 = 0.0;static double f8 = 0.0;
  static double a9 = 0.0;static double b9 = 0.0;static double c9 = 0.0;static double d9 = 0.0;static double e9 = 0.0;static double f9 = 0.0;
  static double a10 = 0.0;static double b10 = 0.0;static double c10 = 0.0;static double d10 = 0.0;static double e10 = 0.0;static double f10 = 0.0;
  
  // Probabilities
  // static double p1 = 0.0;static double p2 = 0.0;static double p3 = 0.0;static double p4 = 0.0;static double p5 = 0.0;static double p6 = 0.0;static double p7 = 0.0;static double p8 = 0.0;static double p9 = 0.0;static double p10 = 0.0;

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    static const char * items[]{"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};
    static int numOfTrans = -1;
    // static bool confirm = false;

    {
      ImGui::Begin("Menu", NULL, ImGuiWindowFlags_AlwaysAutoResize);

      ImGui::Text("Affine transformations:");
      ImGui::Text("x1 = a*x + b*y + e");
      ImGui::Text("y1 = c*x + d*y + f");

      ImGui::Combo("Number of affine transformations", &numOfTrans, items, IM_ARRAYSIZE(items));

      int iter = 0;

      if (iter < numOfTrans+1){
        ImGui::Text("Number %d :", 1);  
        ImGui::InputDouble("a1", &a1); ImGui::SameLine();
        ImGui::InputDouble("b1", &b1); ImGui::SameLine();
        ImGui::InputDouble("c1", &c1); ImGui::SameLine();
        ImGui::InputDouble("d1", &d1); ImGui::SameLine();
        ImGui::InputDouble("e1", &e1); ImGui::SameLine();
        ImGui::InputDouble("f1", &f1); //ImGui::SameLine();
        // ImGui::InputDouble("p1", &p1);
        iter+=1;
      }

      if (iter < numOfTrans+1){
        ImGui::Text("Number %d :", 2);   
        ImGui::InputDouble("a2", &a2); ImGui::SameLine();
        ImGui::InputDouble("b2", &b2); ImGui::SameLine();
        ImGui::InputDouble("c2", &c2); ImGui::SameLine();
        ImGui::InputDouble("d2", &d2); ImGui::SameLine();
        ImGui::InputDouble("e2", &e2); ImGui::SameLine();
        ImGui::InputDouble("f2", &f2); //ImGui::SameLine();
        // ImGui::InputDouble("p2", &p2);
        iter+=1;
      }

      if (iter < numOfTrans+1){
        ImGui::Text("Number %d :", 3);   
        ImGui::InputDouble("a3", &a3); ImGui::SameLine();
        ImGui::InputDouble("b3", &b3); ImGui::SameLine();
        ImGui::InputDouble("c3", &c3); ImGui::SameLine();
        ImGui::InputDouble("d3", &d3); ImGui::SameLine();
        ImGui::InputDouble("e3", &e3); ImGui::SameLine();
        ImGui::InputDouble("f3", &f3); //ImGui::SameLine();
        // ImGui::InputDouble("p3", &p3);
        iter+=1;
      }

      if (iter < numOfTrans+1){
        ImGui::Text("Number %d :", 4);  
        ImGui::InputDouble("a4", &a4); ImGui::SameLine();
        ImGui::InputDouble("b4", &b4); ImGui::SameLine();
        ImGui::InputDouble("c4", &c4); ImGui::SameLine();
        ImGui::InputDouble("d4", &d4); ImGui::SameLine();
        ImGui::InputDouble("e4", &e4); ImGui::SameLine();
        ImGui::InputDouble("f4", &f4); //ImGui::SameLine();
        // ImGui::InputDouble("p4", &p4);
        iter+=1;
      }

      if (iter < numOfTrans+1){
        ImGui::Text("Number %d :", 5);  
        ImGui::InputDouble("a5", &a5); ImGui::SameLine();
        ImGui::InputDouble("b5", &b5); ImGui::SameLine();
        ImGui::InputDouble("c5", &c5); ImGui::SameLine();
        ImGui::InputDouble("d5", &d5); ImGui::SameLine();
        ImGui::InputDouble("e5", &e5); ImGui::SameLine();
        ImGui::InputDouble("f5", &f5); //ImGui::SameLine();
        // ImGui::InputDouble("p5", &p5);
        iter+=1;
      }

      if (iter < numOfTrans+1){
        ImGui::Text("Number %d :", 6);  
        ImGui::InputDouble("a6", &a6); ImGui::SameLine();
        ImGui::InputDouble("b6", &b6); ImGui::SameLine();
        ImGui::InputDouble("c6", &c6); ImGui::SameLine();
        ImGui::InputDouble("d6", &d6); ImGui::SameLine();
        ImGui::InputDouble("e6", &e6); ImGui::SameLine();
        ImGui::InputDouble("f6", &f6);
        // ImGui::InputDouble("p6", &p6);
        iter+=1;
      }

      if (iter < numOfTrans+1){
        ImGui::Text("Number %d :", 7);  
        ImGui::InputDouble("a7", &a7); ImGui::SameLine();
        ImGui::InputDouble("b7", &b7); ImGui::SameLine();
        ImGui::InputDouble("c7", &c7); ImGui::SameLine();
        ImGui::InputDouble("d7", &d7); ImGui::SameLine();
        ImGui::InputDouble("e7", &e7); ImGui::SameLine();
        ImGui::InputDouble("f7", &f7); 
        // ImGui::InputDouble("p7", &p7);
        iter+=1;
      }
      
      if (iter < numOfTrans+1){
        ImGui::Text("Number %d :", 8);  
        ImGui::InputDouble("a8", &a8); ImGui::SameLine();
        ImGui::InputDouble("b8", &b8); ImGui::SameLine();
        ImGui::InputDouble("c8", &c8); ImGui::SameLine();
        ImGui::InputDouble("d8", &d8); ImGui::SameLine();
        ImGui::InputDouble("e8", &e8); ImGui::SameLine();
        ImGui::InputDouble("f8", &f8);
        // ImGui::InputDouble("p8", &p8);
        iter+=1;
      }

      if (iter < numOfTrans+1){
        ImGui::Text("Number %d :", 9);  
        ImGui::InputDouble("a9", &a9); ImGui::SameLine();
        ImGui::InputDouble("b9", &b9); ImGui::SameLine();
        ImGui::InputDouble("c9", &c9); ImGui::SameLine();
        ImGui::InputDouble("d9", &d9); ImGui::SameLine();
        ImGui::InputDouble("e9", &e9); ImGui::SameLine();
        ImGui::InputDouble("f9", &f9); 
        // ImGui::InputDouble("p9", &p9);
        iter+=1;
      }

      if (iter < numOfTrans+1){
        ImGui::Text("Number %d :", 10);  
        ImGui::InputDouble("a10", &a10); ImGui::SameLine();
        ImGui::InputDouble("b10", &b10); ImGui::SameLine();
        ImGui::InputDouble("c10", &c10); ImGui::SameLine();
        ImGui::InputDouble("d10", &d10); ImGui::SameLine();
        ImGui::InputDouble("e10", &e10); ImGui::SameLine();
        ImGui::InputDouble("f10", &f10); 
        // ImGui::InputDouble("p10", &p10);
        iter+=1;
      }

      if (ImGui::Button("Generate")){
        std::cout<<"\nstarted generating"<<std::endl;
        affineTransformations.clear();
        // probabilities.clear();
        
        int i = 0;
        if (i < numOfTrans+1){
          affineTransformations.push_back({a1,b1,c1,d1,e1,f1}); i+=1;
          // probabilities.push_back(p1);
        }
        if (i < numOfTrans+1){
          affineTransformations.push_back({a2,b2,c2,d2,e2,f2}); i+=1;
          // probabilities.push_back(p2);
        }
        if (i < numOfTrans+1){
          affineTransformations.push_back({a3,b3,c3,d3,e3,f3}); i+=1;
          // probabilities.push_back(p3);
        }
        if (i < numOfTrans+1){
          affineTransformations.push_back({a4,b4,c4,d4,e4,f4}); i+=1;
          // probabilities.push_back(p4);
        }
        if (i < numOfTrans+1){
          affineTransformations.push_back({a5,b5,c5,d5,e5,f5}); i+=1;
          // probabilities.push_back(p5);
        }
        if (i < numOfTrans+1){
          affineTransformations.push_back({a6,b6,c6,d6,e6,f6}); i+=1;
          // probabilities.push_back(p6);
        }
        if (i < numOfTrans+1){
          affineTransformations.push_back({a7,b7,c7,d7,e7,f7}); i+=1;
          // probabilities.push_back(p7);
        }
        if (i < numOfTrans+1){
          affineTransformations.push_back({a8,b8,c8,d8,e8,f8}); i+=1;
          // probabilities.push_back(p8);
        }
        if (i < numOfTrans+1){
          affineTransformations.push_back({a9,b9,c9,d9,e9,f9}); i+=1;
          // probabilities.push_back(p9);
        }
        if (i < numOfTrans+1){
          affineTransformations.push_back({a10,b10,c10,d10,e10,f10}); i+=1;
          // probabilities.push_back(p10);
        }

        pointsUpdated = true;

        //Debug
        for (int i = 0; i < affineTransformations.size(); i++)
        {
            std::cout << "Affine Transformation " << i+1 << ": ";
            for (int j = 0; j < affineTransformations[i].size(); j++)
            {
                std::cout << affineTransformations[i][j] << " ";
            }
            std::cout << std::endl;
        }
      }

      ImGui::End();
    }

    // Rendering
    ImGui::Render();

    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);

    if (pointsUpdated) {
      if (isConverging()) {
        // All the affine transformations are convergent
        // Update VAO/VBO for points (since we updated the affine transformations)
        generatePoints();
        glBindVertexArray(VAO_points);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_points);
        glBufferData(GL_ARRAY_BUFFER, points.size()*sizeof(GLdouble), &points[0], GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), (void*)0);
        glEnableVertexAttribArray(0); //Enable first attribute buffer (vertices)
      }
      pointsUpdated = false;
    }

    glUseProgram(shaderProgram);

    // Draw points
    glBindVertexArray(VAO_points);
    glPointSize(WIDTH*HEIGHT/1500000);
    glDrawArrays(GL_POINTS, 0, points.size()/3);

    // glUseProgram(0);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
  }

  // Delete VBO buffer
  glDeleteBuffers(1, &VBO_points);
  
  // Cleanup
  cleanup(window);
  return 0;

}