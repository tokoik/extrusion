#ifndef QUATERNION_H
#define QUATERNION_H

/* クォータニオンの積 */
extern void qmul(double [], const double [], const double []);

/* クォータニオン→回転の変換行列 */
extern void qrot(double [], double []);

#endif
