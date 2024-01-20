using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PilarBuilder : MonoBehaviour
{
    [SerializeField]
    private int m_pilarLineCount = 10;

    [SerializeField]
    private int m_pilarColumnCount = 10;

    [SerializeField]
    private float m_spaceBetweenLine = 1f;

    [SerializeField]
    private float m_spaceBetweenColumn = 1f;

    [SerializeField]
    private GameObject TowerPrefab;

    [SerializeField]
    Camera mainCamera;

    [SerializeField]
    List<Transform> pilars;

    private void Update()
    {
        // Move Pilar around Player View
        BuildWithOrigin(ConvertToNearestTilePosition(mainCamera.transform.position));
    }


    [ContextMenu("Build Tower")]
    void Build()
    {
        BuildWithOrigin(Vector3.zero);
       
    }

    void BuildWithOrigin(Vector3 origin)
    {
        float x = origin.x - m_spaceBetweenColumn * (m_pilarColumnCount / 2);
        float z = origin.z - m_spaceBetweenLine * (m_pilarLineCount / 2);

        Vector3 position = new Vector3(x, 0, z);

        for (int i = 0; i < m_pilarColumnCount; i++)
        {
            for (int j = 0; j < m_pilarLineCount; j++)
            {
                if (pilars.Count > j + i * m_pilarLineCount)
                {
                    pilars[j + i * m_pilarLineCount].transform.position = position;



                    // Desactive Pilar In Center of World of After Min Position
                    if (position.x == 0 && position.z == 0 || position.x < -4990)
                    {
                        pilars[j + i * m_pilarLineCount].gameObject.SetActive(false);
                    }
                    else
                    {
                        pilars[j + i * m_pilarLineCount].gameObject.SetActive(true);
                    }
                }
                else
                {
                    GameObject gameObject = null;
#if UNITY_EDITOR
                    if(!Application.isPlaying)
                        gameObject = (GameObject)UnityEditor.PrefabUtility.InstantiatePrefab(TowerPrefab, transform);
                    else
                        gameObject = (GameObject)Instantiate(TowerPrefab, transform);
#else
                    gameObject = (GameObject)Instantiate(TowerPrefab, transform);
#endif
                    gameObject.transform.position = position;
                    pilars.Add(gameObject.transform);


                    // Desactive Pilar In Center of World
                    if (position.x == 0 && position.z == 0)
                    {
                        gameObject.SetActive(false);
                    }
                    else
                    {
                        gameObject.SetActive(true);
                    }

                }


                position.z += m_spaceBetweenLine;

              

            }
            position.z = origin.z - m_spaceBetweenLine * (m_pilarLineCount / 2);


            position.x += m_spaceBetweenColumn;

        }
    }

    public Vector3 ConvertToNearestTilePosition(Vector3 inputPosition)
    {
        // Calculer l'index de la tuile en divisant la position par la taille de la tuile
        int xIndex = Mathf.RoundToInt(inputPosition.x / m_spaceBetweenColumn);
        int zIndex = Mathf.RoundToInt(inputPosition.z / m_spaceBetweenLine);

        // Convertir l'index en position de la tuile en multipliant par la taille de la tuile
        Vector3 nearestTilePosition = new Vector3(xIndex * m_spaceBetweenColumn, 0.0f, zIndex * m_spaceBetweenLine);

        return nearestTilePosition;
    }
}
