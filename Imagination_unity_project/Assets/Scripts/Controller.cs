using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Controller : MonoBehaviour
{
    [SerializeField]
    private float m_movementSpeed = 10;

    [SerializeField]
    private float m_turningSpeed = 60;

    [SerializeField]
    private Camera m_camera;

    [SerializeField]
    private Vector3 m_cameraOffset;

    [SerializeField]
    private float m_distance;

    [SerializeField]
    private float m_maxZPosition = 5000;

    [SerializeField]
    private float m_minZposition = -5000;

    [Header("Debug")]
    private float time;
    public string hours;

  
   


    private void FixedUpdate()
    {
        MoveCharacter();
    }

    private void Update()
    {
        DebugInfo();

    }


    private void LateUpdate()
    {
        MoveCamera();
    }


    private void MoveCharacter()
    {
        //Rotate
        float horizontal = Input.GetAxis("Horizontal") * m_turningSpeed * Time.deltaTime;
        transform.Rotate(0, horizontal, 0);

        //Translate
        float vertical = Input.GetAxis("Vertical") * m_movementSpeed * Time.deltaTime;
        transform.Translate(0, 0, vertical);

        if (transform.position.z > m_maxZPosition)
        {
            Vector3 pos = transform.position;
            pos.z = m_minZposition;
            transform.position = pos;
        }
        else if(transform.position.z < m_minZposition)
        {
            Vector3 pos = transform.position;
            pos.z = m_maxZPosition;
            transform.position = pos;
        }

    }

    private void MoveCamera()
    {
        if (!m_camera)
            return;
        
       
        //Follow Character
        m_camera.transform.position = transform.position -transform.forward * m_distance + m_cameraOffset;

        // Look Character
        m_camera.transform.LookAt(transform);

    }

    private void DebugInfo()
    {
        time += Time.deltaTime;

        TimeSpan t = TimeSpan.FromSeconds(time);

        hours = string.Format("{0:D2}h:{1:D2}m:{2:D2}s:{3:D3}ms",
                        t.Hours,
                        t.Minutes,
                        t.Seconds,
                        t.Milliseconds);
    }

}
