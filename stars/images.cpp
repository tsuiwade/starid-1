#include "images.h"
#include "globals.h"
#include <cmath>
#include <random>

std::random_device r;
std::default_random_engine e1(r());
std::uniform_real_distribution<double> unitscatter(0, 1);

void stars::Images::get_image_vectors(std::string& imgfile, int imgndx) {
    Eigen::Matrix<double, 28, 28> axjAxiImage = stars::Images::read_images_container(imgfile, imgndx);
    uvecs = Eigen::MatrixXd::Zero(100,3);
    uvecs(0,0) = 0.0; // center star, stars, is implicit in the image
    uvecs(0,1) = 0.0;
    uvecs(0,2) = 1.0;
    int uvecsndx = 1;
    for (int axjndx = 0; axjndx < 28; ++axjndx) {
        for (int axindx = 0; axindx < 28; ++axindx) {
            if (axjAxiImage(axjndx, axindx) > 0) { // there's a star inside axjndx, axindx
                double x = stars::image_pixel_unit_vector_plane * ( -13.5 + (double)axindx );
                double y = stars::image_pixel_unit_vector_plane * ( +13.5 - (double)axjndx );
                uvecs(uvecsndx,0) = x;
                uvecs(uvecsndx,1) = y;
                uvecs(uvecsndx,2) = std::sqrt(1 - x*x - y*y);
                ++uvecsndx;
            }
        }
    }
    uvecs.conservativeResize(uvecsndx, 3);
}

Eigen::Matrix<double, 28, 28> stars::Images::read_images_container(std::string& imgfile, int imgndx) {
    Eigen::Matrix<double, 28, 28> image;
    std::ifstream file (imgfile, std::ios::binary);
    if (file.is_open())
    {
        int magnumimg, imgcnt, axjcnt, axicnt; // local variables, function is static
        file.read((char*) &magnumimg, sizeof(magnumimg));
        magnumimg = reverseInt(magnumimg);
        file.read((char*) &imgcnt, sizeof(imgcnt));
        imgcnt = reverseInt(imgcnt);
        file.read((char*) &axjcnt, sizeof(axjcnt));
        axjcnt = reverseInt(axjcnt); // 28
        file.read((char*) &axicnt, sizeof(axicnt));
        axicnt = reverseInt(axicnt); // 28

        int imgndx = 0;
        while (imgndx < imgndx) {
            for (int axjndx = 0; axjndx < 28; ++axjndx) { // inverted-y-like, row-like
                for (int axindx = 0; axindx < 28; ++axindx) { // x-like, col-like
                    unsigned char temp = 0;
                    file.read((char*) &temp, sizeof(temp));
                }
            }
            ++imgndx;
        }
        // (axjndx inverted-y-like row-like), (axindx x-like col-like) plane
        for (int axjndx = 0; axjndx < 28; ++axjndx) { // inverted-y-like, row-like
            for (int axindx = 0; axindx < 28; ++axindx) { // x-like, col-like
                unsigned char temp = 0;
                file.read((char*) &temp, sizeof(temp));
                image(axjndx, axindx) = (double)temp;
            }
        }
    }
    return image;
}

//void stars::Images::axjAxiImageUpdate(arma::mat& axjAxiImage, stars::Sky& sky, int starndx) {

//    arma::vec pointing(3);
//    pointing(0) = sky.stars[starndx].x;
//    pointing(1) = sky.stars[starndx].y;
//    pointing(2) = sky.stars[starndx].z;
//    std::vector<int> starndxs = sky.starsNearPoint(pointing(0), pointing(1), pointing(2));

//    uvecs.zeros(100,3);
//    int uvecsndx = 0;
//    for (auto ndx : starndxs) {
//        uvecs(uvecsndx,0) = sky.stars[ndx].x;
//        uvecs(uvecsndx,1) = sky.stars[ndx].y;
//        uvecs(uvecsndx,2) = sky.stars[ndx].z;
//        ++uvecsndx;
//    }
//    uvecs.shed_rows(uvecsndx, 99);

//    double yaw = unitscatter(e1) * 2 * M_PI;
//    arma::mat attitude = rotationMatrix(pointing);
//    uvecs = arma::trans( arma::trans(attitude) * arma::trans(uvecs) );

//    axjAxiImage.zeros(); // update axjaxiimage
//    for (int ndx = 0; ndx < uvecsndx; ++ndx) {
//        double x = std::cos(yaw) * uvecs(ndx,0) - std::sin(yaw) * uvecs(ndx,1);
//        double y = std::sin(yaw) * uvecs(ndx,0) + std::cos(yaw) * uvecs(ndx,1);
//        double axi = x + stars::image_radius_unit_vector_plane;
//        double axj = -y + stars::image_radius_unit_vector_plane;
//        int axindx = std::floor( axi / stars::image_pixel_unit_vector_plane );
//        int axjndx = std::floor( axj / stars::image_pixel_unit_vector_plane );
//        if (axjndx < 0 || axjndx > 27) continue;
//        if (axindx < 0 || axindx > 27) continue;
//        axjAxiImage(axjndx, axindx) = 255.0;
//    }
//}

//arma::mat stars::Images::rotationMatrix(arma::vec& pointing) {
//    arma::mat rm;
//    rm.eye(3,3);
//    arma::vec bz = pointing;
//    arma::vec iz = { 0.0, 0.0, 1.0 };
//    arma::vec b1x = arma::cross(bz,iz);
//    arma::vec b1y = arma::cross(bz,b1x);
//    rm.col(0) = arma::normalise(b1x);
//    rm.col(1) = arma::normalise(b1y);
//    rm.col(2) = bz;
//    return rm;
//}

//void stars::Images::writeImages(std::string filename, std::vector<arma::mat> &images) {
//    std::ofstream file (filename, std::ios::binary);
//    int rev_magnumimg = reverseInt(magnumimg);
//    int rev_imgcnt = reverseInt(imgcnt);
//    int rev_axjcnt = reverseInt(axjcnt);
//    int rev_axicnt = reverseInt(axicnt);
//    if (file.is_open()) {
//        file.write((char*) &rev_magnumimg, sizeof(rev_magnumimg));
//        file.write((char*) &rev_imgcnt, sizeof(rev_imgcnt));
//        file.write((char*) &rev_axjcnt, sizeof(rev_axjcnt));
//        file.write((char*) &rev_axicnt, sizeof(rev_axicnt));
//        for (int imgndx = 0; imgndx < imgcnt; ++imgndx) {
//            arma::mat image = images[imgndx];
//            for (int axjndx = 0; axjndx < axjcnt; ++axjndx) {
//                for(int axindx = 0; axindx < axicnt; ++axindx) {
//                    unsigned char temp = (unsigned char) image(axjndx, axindx);
//                    file.write((char*) &temp, sizeof(temp));
//                }
//            }
//        }
//    }
//}

//void stars::Images::readAxjAxiImages(std::string filename, std::vector<arma::mat> &images) {
//    std::ifstream file (filename, std::ios::binary);
//    if (file.is_open())
//    {
//        file.read((char*) &magnumimg, sizeof(magnumimg));
//        magnumimg = reverseInt(magnumimg);
//        file.read((char*) &imgcnt, sizeof(imgcnt));
//        imgcnt = reverseInt(imgcnt);
//        file.read((char*) &axjcnt, sizeof(axjcnt));
//        axjcnt = reverseInt(axjcnt);
//        file.read((char*) &axicnt, sizeof(axicnt));
//        axicnt = reverseInt(axicnt);
//        for(int imgndx = 0; imgndx < imgcnt; ++imgndx) {
//            arma::mat image(axjcnt, axicnt);
//            for(int axjndx = 0; axjndx < axjcnt; ++axjndx) {
//                for(int axindx = 0; axindx < axicnt; ++axindx) {
//                    unsigned char temp = 0;
//                    file.read((char*) &temp, sizeof(temp));
//                    image(axjndx, axindx) = (double) temp;
//                }
//            }
//            images.push_back(image);
//        }
//    }
//}

//void stars::Images::writeLabels(std::string filename, arma::colvec &labels) {
//    std::ofstream file (filename, std::ios::binary);
//    int rev_magnumlab = reverseInt(magnumlab);
//    int rev_imgcnt = reverseInt(imgcnt);
//    if (file.is_open()) {
//        file.write((char*) &rev_magnumlab, sizeof(rev_magnumlab));
//        file.write((char*) &rev_imgcnt, sizeof(rev_imgcnt));
//        for(int imgndx = 0; imgndx < imgcnt; ++imgndx) {
//            unsigned char temp = (unsigned char) labels(imgndx);
//            file.write((char*) &temp, sizeof(temp));
//        }
//    }
//}

//void stars::Images::readLabels(std::string filename, arma::colvec &labels) {
//    std::ifstream file (filename, std::ios::binary);
//    if (file.is_open()) {
//        file.read((char*) &magnumlab, sizeof(magnumlab));
//        magnumlab = reverseInt(magnumlab);
//        file.read((char*) &imgcnt, sizeof(imgcnt));
//        imgcnt = reverseInt(imgcnt);
//        for(int imgndx = 0; imgndx < imgcnt; ++imgndx) {
//            unsigned char temp = 0;
//            file.read((char*) &temp, sizeof(temp));
//            labels(imgndx)= (double) temp;
//        }
//    }
//}

int stars::Images::reverseInt (int i)
{
    unsigned char ch1, ch2, ch3, ch4;
    ch1 = i & 255;
    ch2 = (i >> 8) & 255;
    ch3 = (i >> 16) & 255;
    ch4 = (i >> 24) & 255;
    return((int) ch1 << 24) + ((int)ch2 << 16) + ((int)ch3 << 8) + ch4;
}