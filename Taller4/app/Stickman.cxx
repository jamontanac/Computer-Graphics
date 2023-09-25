#include <iostream>

#include <PUJ_CGraf/AmbientLight.h>
#include <PUJ_CGraf/Controller.h>
#include <PUJ_CGraf/SphericalCamera.h>
#include <PUJ_CGraf/SpotLight.h>
#include <PUJ_CGraf/World.h>

#include <PUJ_CGraf/Object.h>
#include <PUJ_CGraf/Cylinder.h>
#include <PUJ_CGraf/Sphere.h>
#include <fstream>
#include <limits>
#include <sstream>

#include <boost/tokenizer.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/optional/optional.hpp>

/**
 */
class Stickman
    : public PUJ_CGraf::Object
{
public:
    using Self = Stickman;
    using Superclass = PUJ_CGraf::Object;
    using TReal = Superclass::TReal;
    using TNatural = Superclass::TNatural;
    using VReal = Superclass::VReal;
    using VNatural = Superclass::VNatural;

public:
    Stickman(const std::string &in_fname);
    virtual ~Stickman() = default;

    // virtual void keypressed(unsigned char k) override
    // {
    //     if (std::isdigit(k))
    //         this->m_Articulation = k - '0';
    //     else if (k == 'x')
    //         this->m_Joints[this->m_Articulation].rotate(0.1, 1, 0, 0);
    //     else if (k == 'X')
    //         this->m_Joints[this->m_Articulation].rotate(0.1, -1, 0, 0);
    //     else if (k == 'y')
    //         this->m_Joints[this->m_Articulation].rotate(0.1, 0, 1, 0);
    //     else if (k == 'Y')
    //         this->m_Joints[this->m_Articulation].rotate(0.1, 0, -1, 0);
    //     else if (k == 'z')
    //         this->m_Joints[this->m_Articulation].rotate(0.1, 0, 0, 1);
    //     else if (k == 'Z')
    //         this->m_Joints[this->m_Articulation].rotate(0.1, 0, 0, -1);
    // }
    virtual void draw() const override;

    // virtual void read_Json(const boost::property_tree::ptree &tree);
    void print_json(const boost::property_tree::ptree &tree);
    int number_of_children() const;
    void create_articulation(unsigned int n_Joints, float Radius, float Height);
    

protected:
    int m_Articulation{-1};
    PUJ_CGraf::Cylinder Torso;
    PUJ_CGraf::Sphere Head;
    std::vector<PUJ_CGraf::Sphere> m_Joints_arms;
    std::vector<PUJ_CGraf::Sphere> m_Joints_legs;
    std::vector<PUJ_CGraf::Cylinder> m_Arms;
    std::vector<PUJ_CGraf::Cylinder> m_Legs;
    boost::property_tree::ptree stickman_body;

};

// -------------------------------------------------------------------------
int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "Error: please provide the JSON path to draw the sitckman." << std::endl;
        return EXIT_FAILURE;
    }

    std::string jsonPath = argv[1];

    // Configure window
    PUJ_CGraf::Controller::set_shade_type_to_flat();
    PUJ_CGraf::Controller::init(&argc, argv, "Robotic Arm", 700, 700);
    // Configure world
    Stickman body(jsonPath);
    // body.read_Json(jsonPath);

    PUJ_CGraf::World world;
    world.add_child(&body);

    PUJ_CGraf::AmbientLight ambient_light;
    ambient_light.set_color(1, 1, 1);
    world.add_light(&ambient_light);

    PUJ_CGraf::SphericalCamera cam;
    world.set_camera(&cam);

    // Main loop
    PUJ_CGraf::Controller::set_world(&world);
    PUJ_CGraf::Controller::loop();

    return (EXIT_SUCCESS);
}

// -------------------------------------------------------------------------
Stickman::
    Stickman(const std::string &in_fname)
{

    std::ifstream in_file(in_fname.c_str());
    std::istringstream in_str(std::string(std::istreambuf_iterator<char>{in_file}, {}));
    in_file.close();

    try{
        boost::property_tree::read_json(in_str, this->stickman_body);
    }
    catch (const std::exception &err){
        std::cerr << "Error caught: " << err.what() << std::endl;
        // return EXIT_FAILURE;
    }
    if (this->stickman_body.empty()){
        std::cerr << "Error: Failed to read the JSON file or the file is empty" << std::endl;
    }
    print_json(this->stickman_body);

    unsigned int nJoints_arms = 3;
    unsigned int nJoints_legs = 3;

    // create_articulation(nJoints,3,8);
    this->m_Joints_arms.resize(2*nJoints_arms);
    this->m_Joints_legs.resize(2*nJoints_legs);
    this->m_Arms.resize(2*nJoints_arms);
    this->m_Legs.resize(2*nJoints_legs);

    this->m_Joints_legs.shrink_to_fit();
    this->m_Joints_arms.shrink_to_fit();
    this->m_Arms.shrink_to_fit();
    this->m_Legs.shrink_to_fit();

    //draw Torso and head
    float H_torso = 30;
    float R_head=12;
    float R_torso=10;
    
    this->Torso.set_radius(R_torso);
    this->Torso.set_height(H_torso);
    this->Torso.set_center(0, 0, 0);

    this->Head.set_radius(R_head);
    this->Head.set_center(0, 0, 0);

    this->Head.sample(10, 10);
    this->Torso.sample(10, 10);
    
    this->Torso.add_child(&(this->Head),0,0,H_torso+R_head);

    // draw Arms

    float R_arms = 1.5;
    float H_arms = 30;
    for (unsigned int j = 0; j<nJoints_arms;j++)
    {
        this->m_Joints_arms[j].set_radius(R_arms);
        this->m_Joints_arms[j].set_center(0, 0, 0);
        

        this->m_Arms[j].set_center(0, 0, 0);
        this->m_Arms[j].set_radius(R_arms);
        this->m_Arms[j].set_height(H_arms / nJoints_arms);


        this->m_Joints_arms[j].sample(10, 10);
        this->m_Arms[j].sample(10, 10);
        this->m_Joints_arms[j].add_child(&(this->m_Arms[j]));

        this->m_Joints_arms[j+nJoints_arms].set_radius(R_arms);
        this->m_Joints_arms[j+nJoints_arms].set_center(0, 0, 0);

        this->m_Arms[j+nJoints_arms].set_center(0, 0, 0);
        this->m_Arms[j+nJoints_arms].set_radius(R_arms);
        this->m_Arms[j+nJoints_arms].set_height(H_arms / nJoints_arms);
        
        this->m_Joints_arms[j+nJoints_arms].sample(10, 10);
        this->m_Arms[j+nJoints_arms].sample(10, 10);
        this->m_Joints_arms[j+nJoints_arms].add_child(&(this->m_Arms[j+nJoints_arms]));

        if (j > 0)
        {
            this->m_Arms[j - 1]
                .add_child(&(this->m_Joints_arms[j]), 0, 0, H_arms / nJoints_arms);
            this->m_Arms[j+nJoints_arms - 1]
                .add_child(&(this->m_Joints_arms[j+nJoints_arms]), 0, 0, H_arms / nJoints_arms);
        }
        else
        {
            
            this->m_Arms[j].rotate(90, 1, 0, 0);
            this->m_Arms[j+nJoints_arms].rotate(90, 1, 0, 0);
            this->Torso.add_child(&(this->m_Joints_arms[j]), R_torso, 0, 0.9 * H_torso);
            this->Torso.add_child(&(this->m_Joints_arms[j+nJoints_arms]), -R_torso, 0, 0.9 * H_torso);
        }
        
    }
    //draw leg Right
    //same as before

    float R_legs = 1.5;
    float H_legs = 30; 
    for (unsigned int j = 0; j<nJoints_legs;j++)
    {
        this->m_Joints_legs[j].set_radius(R_legs);
        this->m_Joints_legs[j].set_center(0, 0, 0);

        this->m_Legs[j].set_center(0, 0, 0);
        this->m_Legs[j].set_radius(R_legs);
        this->m_Legs[j].set_height(H_legs / nJoints_legs);

        this->m_Joints_legs[j].sample(10, 10);
        this->m_Legs[j].sample(10, 10);
        this->m_Joints_legs[j].add_child(&(this->m_Legs[j]));


        this->m_Joints_legs[j+nJoints_legs].set_radius(R_legs);
        this->m_Joints_legs[j+nJoints_legs].set_center(0, 0, 0);

        this->m_Legs[j+nJoints_legs].set_center(0, 0, 0);
        this->m_Legs[j+nJoints_legs].set_radius(R_legs);
        this->m_Legs[j+nJoints_legs].set_height(H_legs / nJoints_legs);

        this->m_Joints_legs[j+nJoints_legs].sample(10, 10);
        this->m_Legs[j+nJoints_legs].sample(10, 10);
        this->m_Joints_legs[j+nJoints_legs].add_child(&(this->m_Legs[j+nJoints_legs]));
        if (j > 0)
        {
            this->m_Legs[j - 1].add_child(&(this->m_Joints_legs[j]), 0, 0, H_legs / nJoints_legs);
            this->m_Legs[j+nJoints_legs - 1].add_child(&(this->m_Joints_legs[j+nJoints_legs]), 0, 0, H_legs / nJoints_legs);

        }
        else
        {
            this->Torso.add_child(&(this->m_Joints_legs[j]), R_torso * 0.5, 0, -H_torso);
            this->Torso.add_child(&(this->m_Joints_legs[j+nJoints_legs]), -R_torso * 0.5, 0, -H_torso);
        }
    }

    //draw leg Left
    //same as before
    // this->m_Joints_legs[1].set_radius(R_legs);
    // this->m_Joints_legs[1].set_center(0,0,0);

    // this -> m_Legs[1].set_center(0,0,0);
    // this -> m_Legs[1].set_radius(R_legs);
    // this -> m_Legs[1].set_height(H_legs);
    // // this -> m_Arms[4].rotate(90,1,0,0);

    // this->m_Joints_legs[1].sample(10, 10);
    // this->m_Legs[1].sample(10, 10);
    // this->m_Joints_legs[1].add_child(&(this->m_Legs[1]));
    // this->Torso.add_child(&(this-> m_Joints_legs[1]),-R_torso*0.5,0,-H_torso);

    // for (unsigned int j = 0; j < nJoints; ++j)

    // {
    //     this->m_Joints[j].set_radius(5);
    //     this->m_Joints[j].set_center(0, 0, 0);

    //     this->m_Arms[j].set_radius(2);
    //     this->m_Arms[j].set_height(h);
    //     this->m_Arms[j].set_center(0, 0, 0);

    //     this->m_Joints[j].sample(10, 10);
    //     this->m_Arms[j].sample(10, 10);

    //     this->m_Arms[j].add_child(&(this->m_Joints[j]));
    //     if (j > 0)
    //         this->m_Joints[j - 1]
    //             .add_child(&(this->m_Arms[j]), 0, 0, h);
    // } // end for



    this->m_BBox[0] = -50;
    this->m_BBox[1] = -50;
    this->m_BBox[2] = -50;
    this->m_BBox[3] = 50;
    this->m_BBox[4] = 50;
    this->m_BBox[5] = 50;
}

// -------------------------------------------------------------------------
void Stickman::
    draw() const
{
    this-> Torso.draw(); // torso is the general object and everybody depends on him
    // this -> Head.draw();
    // if (this->m_Joints_arms.size() > 0)
        // this->m_Joints_arms[0].draw();
    // if (this->m_Joints_legs.size() > 0)
        // this->m_Joints_legs[0].draw();
}

void Stickman::create_articulation(unsigned int n_Joints,float Radius, float Height)
{
    std::vector<PUJ_CGraf::Sphere> m_Joints;
    std::vector<PUJ_CGraf::Cylinder> m_Arms;

    m_Joints.resize(n_Joints);
    m_Arms.resize(n_Joints);
    m_Joints.shrink_to_fit();
    m_Arms.shrink_to_fit();

    for (unsigned int j = 0; j < n_Joints; ++j)
    {
        m_Joints[j].set_radius(2);
        m_Joints[j].set_center(0, 0, 0);

        m_Arms[j].set_radius(Radius);
        m_Arms[j].set_height(Height);
        m_Arms[j].set_center(0, 0, 0);

        m_Joints[j].sample(10, 10);
        m_Arms[j].sample(10, 10);

        m_Joints[j].add_child(&(m_Arms[j]));
        if (j > 0)
            m_Arms[j - 1].add_child(&(m_Joints[j]), 0, 0, Height);
    } // end for

    // for (unsigned int j = 0; j < n_Joints; ++j)
    // {
    //     this->m_Arms.push_back(m_Arms[j]);
    //     this->m_Joints.push_back(m_Joints[j]);
    // }
}
void Stickman::print_json(const boost::property_tree::ptree &tree)
{

    using _T = boost::property_tree::ptree;
    using _O = boost::optional<const _T &>;
    using _It = _T::const_iterator;
    for (_It i = tree.begin(); i != tree.end(); i++)
    {
        
        _O Joint_node = i->second.get_child_optional("nJoints");
        if (Joint_node){

        std::cout
            << i->first << ": " << i->second.get_value<std::string>()
            << std::endl;
        }
        print_json(i->second);
        // _O radius = i->second.get_child_optional("radius");
        // if (radius)
        // {
        // std::cout << i->first << " " << i->second.get_value << std::endl;
        // }
        // else
        // {
        // read_json(i->second);
        // }
    }
}
// void Stickman::read_Json()
// {

//     using _T = boost::property_tree::ptree;
//     using _O = boost::optional<const _T &>;
//     using _It = _T::const_iterator;
//     for (_It i = this->stickman_body.begin(); i != tree.end(); i++)
//     {
//         _O radius = i->second.get_child_optional("radius");
//         if (radius)
//         {
//             std::cout << i->first << " " << i->second << std::endl;
//         }
//         else
//         {
//             Stickman::read_json(i->second);
//         }
//     }
// }
// eof - Stickman.cxx
